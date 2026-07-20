modded class MissionGameplay
{
	protected bool m_ZenReverseCameraGrainActive;
	protected bool m_ZenReverseCameraHudHidden;

	override void OnMissionStart()
	{
		super.OnMissionStart();

		Zen_EnablePostprocessEffects();

		PPERequesterBase requester = PPERequesterBank.GetRequester(PPERequester_ZenReverseCameraGrain);

		if (requester)
		{
			requester.Start();
			requester.SetRequesterUpdating(true);
		}
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		Zen_UpdateReverseCameraGrain();
	}

	override void OnMissionFinish()
	{
		Zen_RestoreReverseCameraHud();
		Zen_StopReverseCameraGrain();

		PPERequesterBase requester = PPERequesterBank.GetRequester(PPERequesterBank.REQ_ZEN_REVERSE_CAMERA);

		if (requester)
			requester.Stop();

		super.OnMissionFinish();
	}

	void Zen_EnablePostprocessEffects()
	{
		GameOptions gameOptions = new GameOptions();

		if (!gameOptions)
			return;

		gameOptions.Initialize();

		ListOptionsAccess ppOption = ListOptionsAccess.Cast(gameOptions.GetOptionByType(OptionAccessType.AT_POSTPROCESS_EFFECTS));

		if (!ppOption)
			return;

		if (ppOption.GetIndex() == 0)
		{
			ppOption.SetIndex(1);
			gameOptions.Apply();
		}
	}

	void Zen_UpdateReverseCameraGrain()
	{
		if (!g_Game || !g_Game.IsClient())
			return;

		bool shouldEnable = Zen_ShouldShowReverseCameraGrain();

		Zen_UpdateReverseCameraHud(shouldEnable);

		if (shouldEnable)
		{
			Zen_StartReverseCameraGrain();

			PPERequesterBase requester = PPERequesterBank.GetRequester(PPERequesterBank.REQ_ZEN_REVERSE_CAMERA);

			if (requester)
				requester.SetRequesterUpdating(true);
		}
		else
		{
			Zen_StopReverseCameraGrain();
		}
	}

	bool Zen_ShouldShowReverseCameraGrain()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		if (!player)
			return false;

		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();

		if (!vehCmd)
			return false;

		if (vehCmd.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
			return false;

		Transport transport = vehCmd.GetTransport();

		if (!transport)
			return false;

		CarScript car = CarScript.Cast(transport);

		if (!car)
			return false;

		if (!DayZPlayerCamera1stPersonVehicle.CanViewReverseCamera(car))
			return false;

		return car.GetGear() == CarGear.REVERSE;
	}

	void Zen_StartReverseCameraGrain()
	{
		PPERequester_ZenReverseCameraGrain requester = PPERequester_ZenReverseCameraGrain.Cast(PPERequesterBank.GetRequester(PPERequesterBank.REQ_ZEN_REVERSE_CAMERA));

		if (!requester)
			return;

		requester.Start();
		requester.SetReverseCameraEffect(1.0);

		if (!m_ZenReverseCameraGrainActive)
		{
			m_ZenReverseCameraGrainActive = true;
		}
	}

	void Zen_StopReverseCameraGrain()
	{
		PPERequester_ZenReverseCameraGrain requester = PPERequester_ZenReverseCameraGrain.Cast(PPERequesterBank.GetRequester(PPERequesterBank.REQ_ZEN_REVERSE_CAMERA));

		if (requester)
		{
			requester.SetReverseCameraEffect(0.0);
			requester.Stop();
		}

		if (m_ZenReverseCameraGrainActive)
		{
			m_ZenReverseCameraGrainActive = false;
		}
	}

	void Zen_UpdateReverseCameraHud(bool hideHud)
	{
		if (hideHud)
		{
			Zen_HideReverseCameraHud();
			return;
		}

		Zen_RestoreReverseCameraHud();
	}

	void Zen_HideReverseCameraHud()
	{
		if (m_ZenReverseCameraHudHidden)
			return;

		IngameHud hud = IngameHud.Cast(GetHud());

		if (!hud)
			return;

		hud.Show(false);

		m_ZenReverseCameraHudHidden = true;
	}

	void Zen_RestoreReverseCameraHud()
	{
		if (!m_ZenReverseCameraHudHidden)
			return;

		IngameHud hud = IngameHud.Cast(GetHud());

		if (hud)
		{
			if (g_Game && g_Game.GetProfileOption(EDayZProfilesOptions.HUD))
				hud.Show(true);
		}

		m_ZenReverseCameraHudHidden = false;
	}
}