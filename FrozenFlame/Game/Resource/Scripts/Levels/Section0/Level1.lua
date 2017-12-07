GroundType = 0
IsWall = 0
IsEnd = 0

MeleeMinionTable = 
{

}

RangedMinionTable = 
{

}

ExplosiveMinionTable = 
{
	
}

IceBeastTable = 
{
	
}

FireBeastTable = 
{
	
}

IceTrapTable = 
{
	
}

FireTrapTable = 
{
	
}

EnviromentTable = 
{
	{16.0,	0.0,	4.0,	"DeadTree1"},
	{12.0,	0.0,	5.0,	"DeadTree2"},
	{14.0,	0.0,	7.0,	"DeadTree1"},
	{11.0,	0.0,	7.0,	"DeadTree2"},
	{9.0,	0.0,	9.0,	"DeadTree2"},
	{15.0,	0.0,	9.0,	"DeadTree2"},
	{14.0,	0.0,	10.0,	"DeadTree1"},
	{7.0,	0.0,	11.0,	"DeadTree1"},
	{11.0,	0.0,	12.0,	"DeadTree2"},
	{9.0,	0.0,	13.0,	"DeadTree1"},
	{16.0,	0.0,	13.0,	"DeadTree2"},
	{6.0,	0.0,	14.0,	"DeadTree2"},
	{13.0,	0.0,	14.0,	"DeadTree1"},
	{10.0,	0.0,	11.0,	"DeadTree1"},
	{15.0,	0.0,	15.0,	"DeadTree2"},
	{7.0,	0.0,	16.0,	"DeadTree1"},
	{10.0,	0.0,	16.0,	"DeadTree2"},
	{5.0,	-1.0,	8.0,	"Sign", Text = "Sword Attacks",},
}

SceneryTable = 
{
--Lower Section
	{Pos = {-32.0,	-20.0,	0.0,},		YRot = 0.0, Scale = 1.5, ModelName = "LavaPool", IsUpdatable = false,},
	{Pos = {-17.0,	-20.0,	-14.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-19.0,	-20.0,	-12.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-22.0,	-20.0,	-15.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
}

SpawnerTable =
{
	{Pos = {0.0, 2.0, -10.0,},	RespawnDelay = 10.0,	RunOnce = false,	IsGuarded = false,	ReinforceMinimum = 3, ExplosiveChance = 0,	Easy = {6, 6, 6,},	Medium = {6, 6,	6,},	Hard = {6, 6, 6,},},
}