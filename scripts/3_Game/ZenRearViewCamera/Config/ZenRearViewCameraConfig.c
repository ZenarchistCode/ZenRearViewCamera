ref ZenRearViewCameraConfig g_ZenRearViewCameraConfig;

static ZenRearViewCameraConfig GetZenRearViewCameraConfig()
{
	if (!g_ZenRearViewCameraConfig) GetZenConfigRegister().RegisterConfig(ZenRearViewCameraConfig);
	return g_ZenRearViewCameraConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenRearViewCameraConfig);
	}
}

class ZenRearViewCameraConfig : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenRearViewCameraConfig = this;
	}

	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool		ShouldSyncToClient() 		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenRearViewCameraConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenRearViewCameraConfig>.SaveFile(path, this, err);
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenRearViewCameraConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenRearViewCameraConfig>.LoadData(payload, this, err);
	}
	
	// -------------------------
	// CONFIG VARIABLES
	// -------------------------
	ref array<string> RearViewVehicles;
	
	override void SetDefaults()
	{
		RearViewVehicles = new array<string>;
		
		RearViewVehicles.Insert("Offroad_02");
		RearViewVehicles.Insert("Truck_01_Base");
		RearViewVehicles.Insert("ExpansionBus");
		RearViewVehicles.Insert("ExpansionVodnik");
	}
}
