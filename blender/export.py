import os
import Blender
from Blender import Window,Draw,Mesh
import BPyMesh
import bpy
import struct
import math

MODEL_HEADER = 'BDM2'
#MATERIAL_HEADER = 'MATS'
ANIMATION_HEADER = 'ANIM'
VBO_HEADER = 'VBOD'

EXPORT_APPLY_MODIFIERS = 1


def __FloatToHalf(i):
	s = int((i >> 16) & 0x00008000)                   # sign
	e = int(((i >> 23) & 0x000000ff) - (127 - 15))    # exponent
	f = int(i        & 0x007fffff)                    # fraction

	# need to handle NaNs and Inf?
	if e <= 0:
		if e < -10:
			if s:                                              # handle -0.0
				return int(0x8000)
			else:
				return int(0)

		f = (f | 0x00800000) >> (1 - e)
		return int(s | (f >> 13))
	elif (e == 0xff - (127 - 15)):
		if f == 0:                                             # Inf
			return int(s | 0x7c00)
		else:                                                  # NAN
			f >>= 13
			return int(s | 0x7c00 | f | (f == 0))
	else:
		if e > 30:                                            # Overflow
			return int(s | 0x7c00)
		return int(s | (e << 10) | (f >> 13))


def __HalfToFloat(h):
	s = int((h >> 15) & 0x00000001)     # sign
	e = int((h >> 10) & 0x0000001f)     # exponent
	f = int(h &         0x000003ff)     # fraction

	if e == 0:
		if f == 0:
			return int(s << 31)
		else:
			while not (f & 0x00000400):
				f <<= 1
				e -= 1
			e += 1
			f &= ~0x00000400
#			print s,e,f
	elif e == 31:
		if f == 0:
			return int((s << 31) | 0x7f800000)
		else:
			return int((s << 31) | 0x7f800000 | (f << 13))

	e = e + (127 -15)
	f = f << 13

	return int((s << 31) | (e << 23) | f)

def FloatToHalf(f):

#    union { float f; uint32_t i; } v;
#    v.f = i;
#    return floatToHalfI(v.i);

#	v = struct.unpack('f', f)
	str = struct.pack('f', f)
	i = struct.unpack('I', str)
#	h = __FloatToHalf(i[0])
	return int(__FloatToHalf(i[0]))

	# hack to coerce to float
#	str = struct.pack('I',x)
#	f=struct.unpack('f', str)
#	return f

def HalfToFloat(FP16):
#	v = struct.unpack('H', FP16)
#	i = struct.pack('I', FP16)
#	v = struct.unpack('

#	x = __HalfToFloat(v[0])
	x = __HalfToFloat(FP16)

	# hack to coerce to float
	str = struct.pack('I', x)
	f = struct.unpack('f', str)
	return float(f[0])

### relpath ##########################
def relpath(base, target):
    if target[0:2] == "\\\\" or target[0:2] == "//":
        return target[2:len(target)]
    if not os.path.isabs(base):
        base = os.path.abspath(base)
    if not os.path.isabs(target):
        target = os.path.abspath(target)
    if os.sep == "\\":
        base = os.path.normcase(base)
        target = os.path.normcase(target)
    if base == os.sep:
        return '.' + target
    baselist = base.split(os.sep)
    if baselist[-1] == "":
        baselist = baselist[:-1]
    targetlist = target.split(os.sep)
    i = 0
    top = min([len(baselist), len(targetlist)])
    while i < top and baselist[i] == targetlist[i]:
        i+=1
    if i == 0:
        return os.sep.join(targetlist)
    if i == len(baselist):
        return os.sep.join(targetlist[i:])
    else:
        return ('..' + os.sep) * (len(baselist) - i) + os.sep.join(targetlist[i:])

base = "."

# New name based on old with a different extension
def newFName(filename, ext):
#    return filename[: -len(filename.split('.', -1)[-1]) ] + ext
	print("Path base: " + base)
#	newfilename = relpath(base, "model1.bdsm2")
	newfilename = base + "\\model1.bdsm2"
	return newfilename

# Store value in byte, short or integer as needed
def packwrite(file, value):
	if value < 256:
		file.write(struct.pack("<c", chr(value)))
	elif value < 65536:
		file.write(struct.pack("<H", value))
	else:
		file.write(struct.pack("<I", value))

def packmwrite(file, mvalue, value):
	if mvalue < 256:
		file.write(struct.pack("<c", chr(value)))
	elif mvalue < 65536:
		file.write(struct.pack("<H", value))
	else:
		file.write(struct.pack("<I", value))

def rvec3d(v):	return round(v[0], 6), round(v[1], 6), round(v[2], 6)
def rvec2d(v):	return round(v[0], 6), round(v[1], 6)

def setSinglePath(filename):
#	global GUIPARAMS, PARAMS
#	GUIPARAMS['Path'].val = filename
#	PARAMS['ImagePaths'] = [filename]
        base = os.path.dirname(filename)
	print('Base path: ' + base)
	print('Relative path: ' + relpath('.', base))
	base = relpath('.', base)

#	is_editmode = Blender.Window.EditMode()
#	if is_editmode:
	Blender.Window.EditMode(0, '', 0)

	print('Models count: %d' % len(bpy.data.scenes))
	savescene = bpy.data.scenes.active	# save current active scene
	Window.DrawProgressBar( 0.0, "Start exporting..." )
        progress = 0.0
        addProgress = 1.0 / len(bpy.data.scenes)

	USE_GZIP = True

	for scene in bpy.data.scenes:
		progress += addProgress
                Window.DrawProgressBar( progress, 'Exporting model "' + scene.name + '" %.0f %%...' % ( progress * 100.0 ) )

		print('Model "' + scene.name + '":')
		scene.makeCurrent()

		try:
			import gzip
			file_model = gzip.open(base + "\\" + scene.name +".bdsm2.gz", "wb")
#			file_info = gzip.open(base + "\\" + scene.name +".bdsi2.gz", "w")
		except:
			print "Failed to compression modules, exporting uncompressed"
			USE_GZIP = False

			file_model = open(base + "\\" + scene.name +".bdsm2", "wb")

		file_info = open(base + "\\" + scene.name +".bdsi2", "w")

		file_model.write(MODEL_HEADER)
		file_info.write('Model name: "' + scene.name + '"\n')

		ANIMATION = False

# Pass 1 - validata scene for animation required
		render = scene.render
		start =	render.sFrame
		end =	render.eFrame
		if end < start: start, end = end, start
#		if start==end: ANIMATION = False
		saveframe = Blender.Get('curframe')	# save current active frame
		frame = start
#		obj_list = {}
#		used_materials = []
		mesh_list = []
		mesh_by_frame = [[] for i in xrange(end-start+1)]
		omeshes = []
		link_omeshes_mesh = {}
		link_mesh_frame = {}

#		mesh_for_obj = []
#		link_frame_obj_mesh = [{} for i in xrange(end-start+1)]
#		link_obj_mesh = {}
		while frame <= end:
#			file_info.write('  Frame: %d\n' % frame)
			Blender.Set('curframe', frame)

#			objects = scene.objects
#			myobjects = []
#			objects = [object for object in scene.objects if (object.type == 'Mesh') and (not object.restrictRender) and (not object.restrictDisplay) and (1 in object.layers)]
			objects = [object for object in scene.objects if (object.type == 'Mesh') and (not object.restrictRender) and (1 in object.layers)]

			for object in objects:
#				if (object.type == 'Mesh') and (not object.restrictRender) and (1 in object.layers):
#					file_info.write('    Object type: ' + type(object) + '\n')
#					file_info.write('    Object type: ' + object.type + '\n')
#				write('    Objects count (including hidden & unsupported): ' + str(len(objects)) + '\n')
				mesh = BPyMesh.getMeshFromObject(object, None, EXPORT_APPLY_MODIFIERS, False, scene)
				omesh = object.getData(True)
				if (mesh) and (len(mesh.faces)):

#					mesh.transform(object.matrixWorld)
					#print 'Object:', object.name, '   matrix:', object.matrixWorld

					try:
						ipo = object.ipo
					except:
						ipo = None
					if ipo:
#					if 0:
						ANIMATION = True
#						mesh_list.append(mesh)
#						mesh_by_frame[frame-start].append(mesh)
#						link_frame_obj_mesh[frame-start][mesh] = object
#						link_obj_mesh[object] = mesh
#						link_mesh_frame[mesh] = frame
					if 0:
						ANIMATION = True	# Fake command
					else:
#						file_info.write('Original mesh ' + omesh.name + ' for object ' + object.name + '\n')
#						file_info.write('Original mesh ' + omesh + ' for object ' + object.name + '\n')


						if omesh in omeshes:
#							file_info.write('+reusing mesh "' + omesh + '" for object "' + object.name + '\n')
							mesh_by_frame[frame-start].append((link_omeshes_mesh[omesh], object))
						else:
#							file_info.write('+mesh "' + omesh + '" for object "' + object.name + '"\n')
							omeshes.append(omesh)
							link_omeshes_mesh[omesh] = mesh
							mesh_list.append(mesh)
							mesh_by_frame[frame-start].append((mesh, object))
							link_mesh_frame[mesh] = frame


#						if not object in mesh_for_obj:
#
#							if omesh in omeshes:
#								file_info.write('+reusing mesh for link ' + object.name + ' -> ' + link_omeshes_obj[omesh].name +'\n')
#
#								mesh_by_frame[frame-start].append(link_obj_mesh[link_omeshes_obj[omesh]])
#								link_frame_obj_mesh[frame-start][link_obj_mesh[link_omeshes_obj[omesh]]] = object
#								mesh_for_obj.append(object)
#								link_obj_mesh[object] = link_obj_mesh[link_omeshes_obj[omesh]]
#
#							else:
#								file_info.write('+mesh for ' + object.name + '\n')
#
#								omeshes.append(omesh)
#								link_omeshes_obj[omesh] = object
#
#								mesh_list.append(mesh)
#								mesh_by_frame[frame-start].append(mesh)
#
#								link_frame_obj_mesh[frame-start][mesh] = object
#								mesh_for_obj.append(object)
#								link_obj_mesh[object] = mesh
#								link_mesh_frame[mesh] = frame
#						else:
#							file_info.write('+reusing mesh for ' + object.name + '\n')
#
#							mesh_by_frame[frame-start].append(link_obj_mesh[object])
#							link_frame_obj_mesh[frame-start][link_obj_mesh[object]] = object
##							mesh_list.append(link_obj_mesh[object])

#						file_info.write('    Object has a ipo\n')
#					else:
#						file_info.write('    Object has no ipo\n')

					materials = mesh.materials
					if materials:
						# сохраняются материалы только для тех объектов, которым назначен 1 материал
#						if len(materials) == 1:	
#							if not materials[0] in used_materials: used_materials.append(materials[0])
						for material in materials:
							try:
								ipo = material.ipo
							except:
								ipo = None
							if ipo:
#								obj_anim[object] = True;
								ANIMATION = True
#								file_info.write('    Object has a ipo in materials\n')
#							else:
#								file_info.write('    Object has no ipo in materials\n')
#						if ANIMATION: file_info.write('    Object has animation\n')

				#mesh.unlink()

#			file_info.write(' Collect objects count = %d\n' % len(myobjects))

			frame+=1


#		file_info.write('  Used materials: %d\n' % len(used_materials))
#		for i, material in enumerate(used_materials):
#			if material != None:
#				file_info.write('   material "' + material.name + '":')
#				file_info.write('   color: [%.3f,%.3f,%.3f]\n' % (material.R, material.G, material.B))
#			else:
#				file_info.write('   None material\n')
			
# Pass 2 - collect meshes
#		meshes = []
#		frame = start
#		while frame <= end:
#			objects = [object for object in scene.objects if (object.type == 'Mesh') and (not object.restrictRender) and (1 in object.layers)]
#			mesh = BPyMesh.getMeshFromObject(object, None, EXPORT_APPLY_MODIFIERS, False, scene)
#			if mesh:
#			frame+=1
		file_info.write('Original meshes used: %d\n' % len(omeshes))
		for omesh in omeshes:
#			file_info.write(' mesh ' + omesh.name + '\n')
			file_info.write(' mesh ' + omesh + '\n')

		mesh_max = len(mesh_list)

		file_info.write('   meshes: %d\n' % mesh_max )
		file_model.write(struct.pack("<I", mesh_max))

		vert_data = ['' for i in xrange(len(mesh_list))];
		index_data = ['' for i in xrange(len(mesh_list))];

		for mi, mesh in enumerate(mesh_list):
			frame = link_mesh_frame[mesh]
			file_info.write('    [%d] mesh "'%(mi,) + mesh.name + '"  (frame = %d)\n' % frame)
			Blender.Set('curframe', frame)

# Чето у меня не пашут GL_QUADS
#			MESH_QUADS = True
			MESH_QUADS = False
# Проверим состоит ли меш только из квадов
#			for face in mesh.faces:
#				if len(face) == 3:
#					MESH_QUADS = False
#					break

			if not MESH_QUADS:
				file_info.write('    convert to triangles\n')
				tempob = scene.objects.new(mesh)
				oldmode = Mesh.Mode()
				Mesh.Mode(Mesh.SelectModes['FACE'])
				mesh.sel = True
				mesh.quadToTriangle(0)
				mesh.recalcNormals(0)
				scene.objects.unlink(tempob)
				Mesh.Mode(oldmode)

# TBD!!! Нужно еще вставить контроль того что не осталось смешанных face

			EXPORT_UV = True
			EXPORT_COLORS = True

# Генерируем списки вершин и списки индексов
			faceUV = mesh.faceUV
			vertexUV = mesh.vertexUV
			vertexColors = mesh.vertexColors
			if (not faceUV) and (not vertexUV):	EXPORT_UV = False
			if not vertexColors:			EXPORT_COLORS = False
			if not EXPORT_UV:			faceUV = vertexUV = False
			if not EXPORT_COLORS:			vertexColors = False

			color = uvcoord = uvcoord_key = normal = normal_key = None
			verts = [] # list of dictionaries
			# vdict = {} # (index, normal, uv) -> new index
			vdict = [{} for i in xrange(len(mesh.verts))]
			vert_count = 0
			for i, f in enumerate(mesh.faces):
				smooth = f.smooth
				if not smooth:
					normal = tuple(f.no)
					normal_key = rvec3d(normal)
				
				if faceUV:		uv = f.uv
				if vertexColors:	col = f.col
				for j, v in enumerate(f):
					if smooth:
						normal = tuple(v.no)
						normal_key = rvec3d(normal)
			
					if faceUV:
						uvcoord = uv[j][0], 1.0-uv[j][1]
						uvcoord_key = rvec2d(uvcoord)
					elif vertexUV:
						uvcoord = v.uvco[0], 1.0-v.uvco[1]
						uvcoord_key = rvec2d(uvcoord)
			
					if vertexColors:	color = col[j].r, col[j].g, col[j].b
			
					key = normal_key, uvcoord_key, color
			
					vdict_local = vdict[v.index]
			
					if (not vdict_local) or (not vdict_local.has_key(key)):
						vdict_local[key] = vert_count;
						verts.append( (tuple(v.co), normal, uvcoord, color) )
						vert_count += 1

			file_info.write('       Mesh attributes: ')

			vl = len(verts)
			if MESH_QUADS:
				il = len(mesh.faces)*4
			else:
				il = len(mesh.faces)*3

			bits = 0
			if MESH_QUADS:
				bits |= 1
				file_info.write(' [quads]')
			else:
				file_info.write(' [triangles]')

			if EXPORT_UV:
				bits |= 2
				file_info.write(' [texture coordinates]')
			if EXPORT_COLORS:
				bits |= 4
				file_info.write(' [vertex color]')
			if vl < 256:
				file_info.write(' [VB]')
			elif vl < 65536:
				bits |= 8
				file_info.write(' [VS]')
			else:
				bits |= 16
				file_info.write(' [VI]')
			if il < 256:
				file_info.write(' [IB]')
			elif il < 65536:
				bits |= 32
				file_info.write(' [IS]')
			else:
				bits |= 64
				file_info.write(' [II]')

			file_info.write('\n')
			file_model.write(struct.pack("<c", chr(bits)))

			file_info.write('       Mesh verts = %d\n' % vl);
			packwrite(file_model, vl);
#			if vl < 256:
#				file_model.write(struct.pack("<c", chr(vl)))
#			elif vl < 65536:
#				file_model.write(struct.pack("<H", vl))
#			else:
#				file_model.write(struct.pack("<I", vl))

			file_info.write('       Mesh indexes = %d\n' % il)
			packwrite(file_model, il);
#			if il < 256:
#				file_model.write(struct.pack("<c", chr(il)))
#			elif il < 65536:
#				file_model.write(struct.pack("<H", il))
#			else:
#				file_model.write(struct.pack("<I", il))
#			vert_data.append('')
#			file_info.write('    Vertexes:\n')
			vind=0
			for v in verts:
#				file_info.write('       [%d]:' % vind)
				vind += 1
#				file_info.write(' v:(%.3f, %.3f, %.3f)' % (v[0][0], v[0][1], v[0][2]))
				vert_data[mi] += struct.pack('<3H', FloatToHalf(v[0][0]), FloatToHalf(v[0][1]), FloatToHalf(v[0][2]))	# vertex
#				file_info.write(' n:(%.3f, %.3f, %.3f)' % (v[1][0], v[1][1], v[1][2]))
				vert_data[mi] += struct.pack('<3H', FloatToHalf(v[1][0]), FloatToHalf(v[1][1]), FloatToHalf(v[1][2]))	# normals
				if EXPORT_UV:
#					file_info.write(' v:(%.3f, %.3f)' % (v[2][0], v[2][1]))
					vert_data[mi] += struct.pack('<2H', FloatToHalf(v[2][0]), FloatToHalf(v[2][1]))
				if EXPORT_COLORS:
#					file_info.write(' c:(%d, %d, %d, %d)' % (v[3][0], v[3][1], v[3][2], 255))
					vert_data[mi] += struct.pack('<4c', chr(int(v[3][0])), chr(int(v[3][1])), chr(int(v[3][2])), chr(255) ) 
#				file_info.write('\n')

			ri = 0
#			file_info.write('    Indexes:\n')
			iind = 0
			for (i, f) in enumerate(mesh.faces):
#				file.write('%d ' % len(f))
#				file.write(struct.pack("<I", len(f)))
				smooth = f.smooth
				if not smooth: no = rvec3d(f.no)
		
				if faceUV:		uv = f.uv
				if vertexColors:	col = f.col
				for j, v in enumerate(f):
					if f.smooth:		normal = rvec3d(v.no)
					else:			normal = no
					if faceUV:		uvcoord = rvec2d((uv[j][0], 1.0-uv[j][1]))
					elif vertexUV:		uvcoord = rvec2d((v.uvco[0], 1.0-v.uvco[1]))
					if vertexColors:	color = col[j].r, col[j].g, col[j].b
			
#					file.write(struct.pack("<I", vdict[v.index][normal, uvcoord, color]))
					ri += 1;
#					index_data[mi] += struct.pack("<I", vdict[v.index][normal, uvcoord, color])
#					file_info.write('       [%d]: %d\n' % (iind, vdict[v.index][normal, uvcoord, color]))
					iind += 1
					if vl < 256:
						index_data[mi] += struct.pack("<c", chr(vdict[v.index][normal, uvcoord, color]))
					elif vl < 65536:
						index_data[mi] += struct.pack("<H", vdict[v.index][normal, uvcoord, color])
					else:
						index_data[mi] += struct.pack("<I", vdict[v.index][normal, uvcoord, color])

#			file_info.write('       Real indexes = %d\n' % ri)

		if ANIMATION:
			file_info.write('  Animation frames: %d\n' %(end-start+1))
		else:
			end = start
			file_info.write('  No animation (frames: 1)\n')

		file_model.write(ANIMATION_HEADER)
		file_model.write(struct.pack("<I", end-start+1))

		frame = start
		while frame <= end:
			Blender.Set('curframe', frame)
			file_info.write('   frame: %d\n' % frame )

			mif = len(mesh_by_frame[frame-start])
			file_info.write('     meshes in frame: %d\n' % mif)
			packmwrite(file_model, mesh_max, mif);
#			if mif < 256:
#				file_model.write(struct.pack("<c", chr(mif)))
#			elif mif < 65536:
#				file_model.write(struct.pack("<H", mif))
#			else:
#				file_model.write(struct.pack("<I", mif))

			for mesh,mobject in mesh_by_frame[frame-start]:
				i = mesh_list.index(mesh)
				mcolor = [255, 255, 255, 255]

				materials = mesh.materials
				if materials:
#					matidx = link_frame_obj_mesh[frame-start][mesh].activeMaterial
#					if materials[matidx]:
#						mcolor = [int(materials[matidx].R*255), int(materials[matidx].G*255), int(materials[matidx].B*255), int(255) ]
					if materials[0]:
						mcolor = [int(materials[0].R*255), int(materials[0].G*255), int(materials[0].B*255), int(255) ]

				for x in xrange(4):
					if mcolor[x]<0: mcolor[x] = 0
					if mcolor[x]>255: mcolor[x] = 255

#				mobject = link_frame_obj_mesh[frame-start][mesh]
				file_info.write('     mesh "' + mesh.name + '"[%d] for object "'%(i,) + mobject.name + '"' )
				packmwrite(file_model, mesh_max, i)
				file_info.write('  color: [%d,%d,%d,%d]\n' % tuple(mcolor) )
#				file_model.write(struct.pack("<I", (i)))
				file_model.write(struct.pack("<4c", chr(mcolor[0]),chr(mcolor[1]),chr(mcolor[2]),chr(mcolor[3])))

				file_info.write('       matrix:');
				file_info.write(' [[%.2f,%.2f,%.2f,%.2f]' % tuple(mobject.matrixWorld[0]))
				file_info.write(' [%.2f,%.2f,%.2f,%.2f]' % tuple(mobject.matrixWorld[1]))
				file_info.write(' [%.2f,%.2f,%.2f,%.2f]' % tuple(mobject.matrixWorld[2]))
				file_info.write(' [%.2f,%.2f,%.2f,%.2f]]\n' % tuple(mobject.matrixWorld[3]))
				file_model.write(struct.pack("<4f", mobject.matrixWorld[0][0], mobject.matrixWorld[0][1], mobject.matrixWorld[0][2], mobject.matrixWorld[0][3]))
				file_model.write(struct.pack("<4f", mobject.matrixWorld[1][0], mobject.matrixWorld[1][1], mobject.matrixWorld[1][2], mobject.matrixWorld[1][3]))
				file_model.write(struct.pack("<4f", mobject.matrixWorld[2][0], mobject.matrixWorld[2][1], mobject.matrixWorld[2][2], mobject.matrixWorld[2][3]))
				file_model.write(struct.pack("<4f", mobject.matrixWorld[3][0], mobject.matrixWorld[3][1], mobject.matrixWorld[3][2], mobject.matrixWorld[3][3]))


#						file.write(' color: [%.3f,%.3f,%.3f]\n' % (material.R, material.G, material.B))
#			for mesh, object in link_frame_obj_mesh[frame-start].items():
#				file_info.write('     mesh "' + mesh.name + '" for object "' + object.name + '"\n' )
			frame+=1

# Save vbo
		file_model.write(VBO_HEADER)
		for i in xrange(len(mesh_list)):
			file_info.write('   Mesh %d:\n' % i)
			file_model.write(vert_data[i])
			file_model.write(index_data[i])
			file_info.write('     Vertex data size = %d bytes\n' % len(vert_data[i]))
			file_info.write('     Index data size = %d bytes\n' % len(index_data[i]))
#		vert_data = ['' for i in xrange(len(mesh_list))];
#		index_data = ['' for i in xrange(len(mesh_list))];



		Blender.Set('curframe', saveframe)	# restore current active frame
#...
		file_info.close();
		file_model.close();

	savescene.makeCurrent()			# restore active scene
	Window.DrawProgressBar( 1.0, "Finished!" )

#	print('Model file name for save:' + newFName(filename, 'bdsm2'))
	return


#Window.FileSelector(setSinglePath, 'Choose path for exporting', Blender.sys.expandpath('models'))
Window.FileSelector(setSinglePath, 'Choose path for exporting', 'models')
#Window.FileSelector(lambda s:luxProp(scn, "lux", "").set(Blender.sys.dirname(s)+os.sep), "Select file in Lux path")
Window.RedrawAll()
