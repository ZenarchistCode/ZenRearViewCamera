modded class DayZPlayerCamera1stPersonVehicle
{
	static const string ZEN_CAR_REVERSE_CAMERA_START_POINT = "reverse_camera_start";
	static const string ZEN_CAR_REVERSE_CAMERA_END_POINT = "reverse_camera_end";

	static const float ZEN_GENERIC_REVERSE_CAMERA_HEIGHT_FRACTION = 0.42;
	static const float ZEN_GENERIC_REVERSE_CAMERA_REAR_OFFSET = 0.25;
	static const float ZEN_GENERIC_REVERSE_CAMERA_LOOK_DISTANCE = 7.0;
	static const float ZEN_GENERIC_REVERSE_CAMERA_LOOK_DOWN = 0.35;

	static bool CanViewReverseCamera(CarScript car)
	{
        #ifdef ZenModPack
        if (!ZenModEnabled("ZenRearViewCamera"))
            return false;
        #endif
        
		if (!car)
			return false;

		if (Zen_HasReverseCameraMemoryPointsStatic(car))
			return true;

		foreach (string s : GetZenRearViewCameraConfig().RearViewVehicles)
		{
			if (car.IsKindOf(s))
				return true;
		}

		return false;
	}

	static bool Zen_HasReverseCameraMemoryPointsStatic(CarScript car)
	{
		if (!car)
			return false;

		if (car.GetMemoryPointIndex(ZEN_CAR_REVERSE_CAMERA_START_POINT) < 0)
			return false;

		if (car.GetMemoryPointIndex(ZEN_CAR_REVERSE_CAMERA_END_POINT) < 0)
			return false;

		return true;
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		CarScript car = Zen_GetCarReverseCameraVehicle();

		if (!car)
			return;

		if (!Zen_IsCarInReverse(car))
			return;

		vector cameraStartWorld;
		vector cameraEndWorld;

		if (!Zen_GetCarReverseCameraWorldPositions(car, cameraStartWorld, cameraEndWorld))
			return;

		Zen_ApplyCarReverseCamera(car, cameraStartWorld, cameraEndWorld, pOutResult);
	}

	CarScript Zen_GetCarReverseCameraVehicle()
	{
		if (!m_pPlayer)
			return NULL;

		HumanCommandVehicle vehCmd = m_pPlayer.GetCommand_Vehicle();

		if (!vehCmd)
			return NULL;

		if (vehCmd.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
			return NULL;

		Transport transport = vehCmd.GetTransport();

		if (!transport)
			return NULL;

		CarScript car = CarScript.Cast(transport);

		if (!car)
			return NULL;

		if (CanViewReverseCamera(car))
			return car;

		return NULL;
	}

	bool Zen_IsCarInReverse(CarScript car)
	{
		if (!car)
			return false;

		return car.GetGear() == CarGear.REVERSE;
	}

	bool Zen_GetCarReverseCameraWorldPositions(CarScript car, out vector cameraStartWorld, out vector cameraEndWorld)
	{
		if (!car)
			return false;

		if (Zen_GetCarReverseCameraMemoryPointWorldPositions(car, cameraStartWorld, cameraEndWorld))
			return true;

		if (Zen_GetCarReverseCameraBoundsWorldPositions(car, cameraStartWorld, cameraEndWorld))
			return true;

		return false;
	}

	bool Zen_GetCarReverseCameraMemoryPointWorldPositions(CarScript car, out vector cameraStartWorld, out vector cameraEndWorld)
	{
		if (!Zen_HasReverseCameraMemoryPointsStatic(car))
			return false;

		vector cameraStartLocal = car.GetMemoryPointPos(ZEN_CAR_REVERSE_CAMERA_START_POINT);
		vector cameraEndLocal = car.GetMemoryPointPos(ZEN_CAR_REVERSE_CAMERA_END_POINT);

		cameraStartWorld = car.ModelToWorld(cameraStartLocal);
		cameraEndWorld = car.ModelToWorld(cameraEndLocal);

		return true;
	}

	bool Zen_GetCarReverseCameraBoundsWorldPositions(CarScript car, out vector cameraStartWorld, out vector cameraEndWorld)
	{
		if (!car)
			return false;

		vector minMax[2];

		if (!car.GetCollisionBox(minMax))
			return false;

		vector min = minMax[0];
		vector max = minMax[1];

		float width = max[0] - min[0];
		float height = max[1] - min[1];
		float length = max[2] - min[2];

		if (width <= 0.1)
			return false;

		if (height <= 0.1)
			return false;

		if (length <= 0.1)
			return false;

		float centerX = (min[0] + max[0]) * 0.5;
		float cameraY = min[1] + (height * ZEN_GENERIC_REVERSE_CAMERA_HEIGHT_FRACTION);
		float rearZ = min[2];

		vector cameraStartLocal = Vector(centerX, cameraY, rearZ - ZEN_GENERIC_REVERSE_CAMERA_REAR_OFFSET);
		vector cameraEndLocal = Vector(centerX, cameraY - ZEN_GENERIC_REVERSE_CAMERA_LOOK_DOWN, rearZ - ZEN_GENERIC_REVERSE_CAMERA_LOOK_DISTANCE);

		cameraStartWorld = car.ModelToWorld(cameraStartLocal);
		cameraEndWorld = car.ModelToWorld(cameraEndLocal);

		return true;
	}

	void Zen_ApplyCarReverseCamera(CarScript car, vector cameraStartWorld, vector cameraEndWorld, out DayZPlayerCameraResult pOutResult)
	{
		if (!car)
			return;

		if (!m_pPlayer)
			return;

		vector playerTransform[4];
		m_pPlayer.GetTransform(playerTransform);

		vector cameraDirectionWorld = cameraEndWorld - cameraStartWorld;

		if (cameraDirectionWorld.Length() < 0.001)
			return;

		vector cameraStartPlayerLocal = cameraStartWorld.InvMultiply4(playerTransform);
		vector cameraDirectionPlayerLocal = cameraDirectionWorld.InvMultiply3(playerTransform);

		float yaw = Math.Atan2(cameraDirectionPlayerLocal[0], cameraDirectionPlayerLocal[2]) * Math.RAD2DEG;

		float horizontalLength = Math.Sqrt((cameraDirectionPlayerLocal[0] * cameraDirectionPlayerLocal[0]) + (cameraDirectionPlayerLocal[2] * cameraDirectionPlayerLocal[2]));
		float pitch = Math.Atan2(cameraDirectionPlayerLocal[1], horizontalLength) * Math.RAD2DEG;

		vector cameraAngles = Vector(yaw, pitch, 0);

		Math3D.YawPitchRollMatrix(cameraAngles, pOutResult.m_CameraTM);

		pOutResult.m_CameraTM[3] = cameraStartPlayerLocal;

		pOutResult.m_iDirectBone = -1;
		pOutResult.m_iDirectBoneMode = 0;

		pOutResult.m_fUseHeading = 0.0;
		pOutResult.m_fInsideCamera = 0.0;
		pOutResult.m_fDistance = 0.0;
		pOutResult.m_fNearPlane = 0.03;
		pOutResult.m_fIgnoreParentRoll = 0.0;
		pOutResult.m_CollisionIgnoreEntity = car;
		pOutResult.m_bUpdateEveryFrame = true;
	}
}