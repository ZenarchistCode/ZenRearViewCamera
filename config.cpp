/*
	(c) 2026 | ZenRearViewCamera | Zenarchist
*/

class CfgPatches
{
	class ZenRearViewCamera
	{
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class ZenRearViewCamera
	{
		author = "Zenarchist";
		type = "mod";
		version = "1.0";
		defines[]=
		{
			//"ZenRearViewCameraDebug"
		};
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenRearViewCamera/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenRearViewCamera/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = 
				{ 
					"ZenRearViewCamera/scripts/5_Mission"
				};
			};
		};
	};
};
