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
	{ -10, 0, 0 },
	{ 10, 0, 0 },
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
	{ 2, 	0, 	16, 	"DeadTree1" },

	{ -2, 	0, 	12, 	"DeadTree1" },

	{ -6,	0,	8,	"PineTree" },

	{ -10,	0,	4,	"PineTree" },

	{ -14, 	0, 	0, 	"DeadTree1" },
}

SceneryTable = 
{
	{ Pos = { -2, 	0, 	16, },	YRot = nil, Scale = 1.0, ModelName = "PineTree" ,	IsUpdatable = false, },
	{ Pos = { -6, 	0, 	16, },	YRot = nil, Scale = 1.0, ModelName = "PineTree" ,	IsUpdatable = false, },
	{ Pos = { -6,	0,	12,	},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1",	IsUpdatable = false,  },
	{ Pos = { -10, 	0, 	16, },	YRot = nil, Scale = 1.0, ModelName = "DeadTree1",	IsUpdatable = false,  },
	{ Pos = { -10, 	0, 	12, },	YRot = nil, Scale = 1.0, ModelName = "PineTree" ,	IsUpdatable = false, },
	{ Pos = { -10,	0,	8,	},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1",	IsUpdatable = false,  };
	{ Pos = { -14, 	0, 	16, },	YRot = nil, Scale = 1.0, ModelName = "DeadTree1",	IsUpdatable = false,  },
	{ Pos = { -14, 	0, 	12, },	YRot = nil, Scale = 1.0, ModelName = "PineTree" ,	IsUpdatable = false, },
	{ Pos = { -14, 	0, 	8,	},	YRot = nil, Scale = 1.0, ModelName = "DeadTree1",	IsUpdatable = false,  },
	{ Pos = { -14, 	0, 	4,	},	YRot = nil, Scale = 1.0, ModelName = "PineTree" ,	IsUpdatable = false, },
	
-- Lower Section
	{Pos = {-17.0,	-20.0,	-17.0,},		YRot = 0.0, Scale = 25.0, ModelName = "SnowPatch", IsUpdatable = false,},
}

SpawnerTable =
{
	{Pos = {-13.0, 0.0, -10.0,},	RespawnDelay = 5.0,	RunOnce = false,	IsGuarded = false,	ReinforceMinimum = 10, ExplosiveChance = 2,	Easy = {6, 6, 6,},	Medium = {6, 6,	6,},	Hard = {6, 6, 6,},},
}