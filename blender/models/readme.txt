��࠭�祭��:
�᫨ ������ ��ꥪ�� �����祭� ����� ������ ���ਠ��, � ��ꥪ� ���������� � "Vertex color attribute"


--------------------------
��ଠ� ������:

4 ���� - ��������� 䠩�� "BDM2"

4 ���� - ���-�� ��襩 (meshes)

meshes * (tmeshes) ���� - ��ਡ��� ��襩 (�� ᠬ� �����):
{
	1 ���� - ⨯ � ��ਡ��� ��� X:
		���� 0:
			��� 0 - TRIANGLE[0]/QUAD[1]
			��� 1 - Vertex texture coordinate attribute
			��� 2 - Vertex color attribute
			���� 3:4
				00 - ���-�� ���設 ��� 0..255		[VB]
			        01 - ���-�� ���設 ��� 256..65535	[VS]
			        1x - ���-�� ���設 ���� >65535		[VI]
			���� 5:6
				00 - ���-�� �����ᮢ ��� 0..255	[IB]
			        01 - ���-�� �����ᮢ ��� 256..65535	[IS]
			        1x - ���-�� �����ᮢ ���� >65535	[II]
[TBD]			��� 7 - ⨯ ������ 0 - HALF_FLOAT / 1 - FLOAT
	(1[VB], 2[VS] ��� 4[VI]) ����(�) - ����� ���設 ��� ab[mesh]
	(1[IB], 2[IS] ��� 4[II]) ����(�) - ����� �����ᮢ ��� eab[mesh]
} tmeshes[meshes]

----
4 ���� - "ANIM"
4 ���� - ���-�� ���஢ �����樨 (frames)

����� ����:
{
	1(meshes<256), 2(meshes<65536) ��� 4(meshes>65535) ����(�) - ���-�� ��襩 � ���� (meshes_in_frame)
	meshes_in_frame * (t_mesh_inframe) - ���ଠ�� � ����
	{
		1(meshes<256), 2(meshes<65536) ��� 4(meshes>65535 ����(�) - ������ ��� X
		4 ���� - RGBA ���ਠ�� 0 (�᫨ � ��ꥪ� ���� ���ਠ�)	(�������� �����筮��� ��� ��襩 ����� �� �㦥� ���ਠ�)
	} t_mesh_inframe[meshes_in_frame]
} tframe[frames]

4 ���� - "VBOD"

-- ����� ��� ����㧪� � vbo
{
	ab[mesh] ���� - ����� ���設 � ��ਡ�⮢ ���設 (Array buffer) *
	eab[mesh] ���� - ����� �����ᮢ (Element array buffer) **
} mesh_data[meshes]

�ਬ�砭��
* ��ଠ� �࠭���� ������ ���設, ��ଠ��� � ⥪������ ���न��� - HALF_FLOAT (16 ���), 梥� - 4x8 ����
** ��ଠ� �࠭���� ������ �����ᮢ ������ �� ������⢠ ���設: (VB: BYTE, VS: SHORT, VI: INT)

-----------------
���쭥�訥 ����:
-----------------
��३� �� blender 2.5 (��� �⮮��� ���᭮��, ��� � ��� �� ���� �����誨 ࠤ� ������ �⮨� ��३�)

--------------------------
��⨬���樨 �� �������� � ��ꥬ� ��襩:
��� �����樨 ��襩 �⫨������ ⮫쪮 ����楩 �८�ࠧ������ (ᬥ饭��, ������ ����⠡) �࠭��� ⮫쪮 ᠬ
��� � ��᫥����⥫쭮��� ����� �८�ࠧ������ (��⠪� 16 䫮�⮢ �� ��� �� ⮣� �⮨�).
����� ���� ����� �룫拉�� ⠪:
{
	1(meshes<256), 2(meshes<65536) ��� 4(meshes>65535) ����(�) - ���-�� ��襩 � ���� (meshes_in_frame)
	meshes_in_frame * (t_mesh_inframe) - ���ଠ�� � ����
	{
		1(meshes<256), 2(meshes<65536) ��� 4(meshes>65535 ����(�) - ������ ��� X
		4 ���� - RGBA ���ਠ�� 0 (�᫨ � ��ꥪ� ���� ���ਠ�)	(�������� �����筮��� ��� ��襩 ����� �� �㦥� ���ਠ�)
		16*4 ���� - ����� �८�ࠧ������ ���.
	} t_mesh_inframe[meshes_in_frame]
} tframe[frames]


����拉�� ���.

http://www.opengl.org/registry/specs/EXT/bindable_uniform.txt

-----------------------------------------------------------
���������� ��ଠ�� � ���� float (� � ������ � HALF_FLOAT, ��...)

��᪮��� ��ଠ�� �����筠�, � ��� �����筮 ���������� ⮫쪮 ���� ��� ���������� � ���� nz 

�������: 
float nx         = 0.5 * ( n.x + 1 );       // remap from [-1,1] to [0,1]
float ny         = 0.5 * ( n.y + 1 );       // remap from [-1,1] to [0,1]
float floatValue = fract ( 100.0 * nx ) + ny / 1000.0;

if ( n.z < 0.0 )                            // floatValue cannot be greater than 101.0
    floatValue += 200;

��ᯠ�����: 

bool	neg = false;

if ( floatValue > 102.0 )
{
    neg         = true;
    floatValue -= 100.0;
}

float nx = 2.0 * fract ( floatValue ) * 1000.0 - 1.0;
float ny = 2.0 * floor ( floatValue ) * 0.01 - 1.0;
vec3  n = vec3 ( nx, ny, sqrt ( 1.0 - nx * nx - ny * ny );

if ( neg )
    n.z = -n.z;
