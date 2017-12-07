GroundType = 0
IsWall = 0
IsEnd = 0

MeleeMinionTable = 
{

}

RangedMinionTable = 
{
	{3.0,	6.0,	-14.0,	1.0},
	{-3.0,	6.0,	-14.0,	1.0},
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
	{1.0,	0.0,	-11.0,	"Rock1"},
	
	{4.0,	0.0,	-11.0,	"Rock2"},
	{3.0,	0.0,	-14.0,	"Rock3"},
	{7.0,	0.0,	-14.0,	"Rock2"},
	{8.0,	0.0,	-16.0,	"Rock2"},
	
	{-4.0,	0.0,	-11.0,	"Rock2"},
	{-3.0,	0.0,	-14.0,	"Rock3"},
	{-7.0,	0.0,	-14.0,	"Rock2"},
	{-8.0,	0.0,	-16.0,	"Rock2"},	
}

SceneryTable = 
{
--Lower Section
	{Pos = {-32.0,	-20.0,	0.0,},		YRot = 0.0, Scale = 1.8, ModelName = "LavaPool", IsUpdatable = false,},
	{Pos = {-17.0,	-20.0,	-14.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-19.0,	-20.0,	-12.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-22.0,	-20.0,	-15.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
--Upper Section(In Between Rocks)	
	{Pos = {-3.0,	0.0,	-19.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {5.0,	0.0,	-28.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {3.0,	0.0,	-22.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {1.5,	0.0,	-30.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-0.5,	0.0,	-22.0,},	YRot = nil, Scale = 1.0, ModelName = "PineTree", IsUpdatable = false,},
	{Pos = {1.5,	0.0,	-26.0,},	YRot = nil, Scale = 1.0, ModelName = "PineTree", IsUpdatable = false,},
	{Pos = {-3.0,	0.0,	-23.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-3.0,	0.0,	-27.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-3.0,	0.0,	-31.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-1.5,	0.0,	-34.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1", IsUpdatable = false,},
	{Pos = {-1.5,	0.0,	-20.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree2", IsUpdatable = false,},
	{Pos = {1.5,	0.0,	-17.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree2", IsUpdatable = false,},
	{Pos = {1.0,	0.0,	-32.0,},	YRot = nil, Scale = 1.0, ModelName = "DeadTree2", IsUpdatable = false,},
}

SpawnerTable =
{
	{Pos = {10.0, 2.0, -10.0,},	RespawnDelay = 5.0,	RunOnce = false,	IsGuarded = false,	ReinforceMinimum = 10, ExplosiveChance = 30,	Easy = {6, 6, 6,},	Medium = {6, 6,	6, 6, 6, 6,},	Hard = {6, 6, 6,},},
	{Pos = {-10.0, 2.0, -10.0,},	RespawnDelay = 5.0,	RunOnce = false,	IsGuarded = false,	ReinforceMinimum = 10, ExplosiveChance = 30,	Easy = {6, 6, 6,},	Medium = {6, 6,	6, 6, 6, 6,},	Hard = {6, 6, 6,},},
	
}