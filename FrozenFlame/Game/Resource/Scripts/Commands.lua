function Hello()
	print("Hello Lua")
end

function add(a, b)
	print(a + b)
end

function PrintTwoStrings(a, b)
	print(a)
	print(b)
end

function NameAge(a, b)
	print(a)
	print(b)
end

function LLoadLevel()
	if IsEnd == 1 then
		MakeWizard(Wizard)
	end
	
	for k in pairs(MeleeMinionTable) do
		MakeMeleeMinion(MeleeMinionTable[k])
	end
	
	for k in pairs(RangedMinionTable) do
		MakeRangedMinion(RangedMinionTable[k])
	end
	
	for k in pairs(ExplosiveMinionTable) do
		MakeExplosiveMinion(ExplosiveMinionTable[k])
	end
	
	for k in pairs(IceBeastTable) do
		MakeIceBeast(IceBeastTable[k])
	end
	
	for k in pairs(FireBeastTable) do
		MakeFireBeast(FireBeastTable[k])
	end
	
	for k in pairs(IceTrapTable) do
		MakeIceTrap(IceTrapTable[k])
	end
	
	for k in pairs(FireTrapTable) do
		MakeFireTrap(FireTrapTable[k])
	end
	
	for k in pairs(EnviromentTable) do
		MakeEnviromentObject(EnviromentTable[k])
	end
	
	for k in pairs(SceneryTable) do
		MakeSceneryObject(SceneryTable[k])
	end
	
		-- Load Spawners Last --
	for k in pairs(SpawnerTable) do
		MakeSpawner(SpawnerTable[k])
	end	
end

function Next()
	NextSection()
end

function Prev()
	PrevSection()
end

function Goto(a)
	GoToLevel(a)
end

function Reload()
	ReloadSection()
end

function ReloadEnemies()
	ReloadEnemies()
end

function ForceBreak()
	ForceBreak()
end

function Rage()
	EverlastingRage();
end

function SetWizardHP(a)
	SetWizardHP(a);
end

function Render(a)
	Render(a);
end