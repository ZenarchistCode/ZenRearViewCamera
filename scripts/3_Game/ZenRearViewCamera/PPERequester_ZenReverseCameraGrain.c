class PPERequester_ZenReverseCameraGrain extends PPERequester_GameplayBase
{
	void SetReverseCameraEffect(float value)
	{
		value = Math.Clamp(value, 0.0, 1.0);

		float saturation = 1.0 - value;
		float grainSharpness = 5.0 + (value * 4.0);
		float grainSize = 1.0;
		float noise = value * 10.0;

		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_SATURATION, true, saturation, PPEGlow.L_22_BLOODLOSS, PPOperators.SET);

		SetTargetValueFloat(PostProcessEffectType.FilmGrain, PPEFilmGrain.PARAM_SHARPNESS, false, grainSharpness, PPEFilmGrain.L_1_NVG, PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.FilmGrain, PPEFilmGrain.PARAM_GRAINSIZE, false, grainSize, PPEFilmGrain.L_2_NVG, PPOperators.SET);
		SetTargetValueFloat(PPEExceptions.NVLIGHTPARAMS, PPELightIntensityParamsNative.PARAM_NOISE_MULT, false, noise, PPELightIntensityParamsNative.L_1_NVG, PPOperators.HIGHEST);

		SetRequesterUpdating(true);
	}
}

modded class PPERequesterBank
{
	static const int REQ_ZEN_REVERSE_CAMERA = RegisterRequester(PPERequester_ZenReverseCameraGrain);
}