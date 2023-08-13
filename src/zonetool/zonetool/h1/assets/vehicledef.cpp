#include "std_include.hpp"
#include "vehicledef.hpp"

namespace zonetool::h1
{

#define VEHICLE_READ_FIELD(__type__, __field__) \
	if (!data[#__field__].is_null()) *(__type__*)(&vehicle->__field__) = (__type__)data[#__field__].get<__type__>()

#define VEHICLE_READ_FIELD_ARR(__type__, __field__, __size__) \
	if (!data[#__field__].is_null()) \
	{ \
		for (auto idx##__field__ = 0; idx##__field__ < __size__; idx##__field__++) \
		{ \
			*(__type__*)(&vehicle->__field__[idx##__field__]) = (__type__)data[#__field__][idx##__field__].get<__type__>(); \
		} \
	}

#define VEHICLE_READ_STRING(__field__) \
	if (!data[#__field__].is_null()) vehicle->__field__ = mem->duplicate_string(data[#__field__].get<std::string>())

#define VEHICLE_READ_ASSET(__type__, __datafield__, __field__) \
	if (!data[#__field__].is_null() && data[#__field__].is_string()) \
	{ \
		auto asset##__field__ = data[#__field__].get<std::string>(); \
		if (asset##__field__.empty()) \
		{ \
			vehicle->__field__ = nullptr; \
		} \
		else \
		{ \
			vehicle->__field__ = db_find_x_asset_header(XAssetType::__type__, asset##__field__.data(), 1).__datafield__; \
		} \
	} \
	else \
	{ \
		vehicle->__field__ = nullptr; \
	}

#define VEHICLE_READ_ASSET_ARR(__type__, __datafield__, __field__, __struct__, __size__) \
	if (!data[#__field__].is_null()) \
	{ \
		for (auto idx##__field__ = 0; idx##__field__ < __size__; idx##__field__++) \
		{ \
			auto asset##__field__ = data[#__field__][idx##__field__].get<std::string>(); \
			if (asset##__field__.empty()) \
			{ \
				vehicle->__field__[idx##__field__] = nullptr; \
			} \
			else \
			{ \
				vehicle->__field__[idx##__field__] = db_find_x_asset_header(XAssetType::__type__, asset##__field__.data(), 1).__datafield__; \
			} \
		} \
	} \

#define VEHICLE_READ_VALUE_ARR(__type__, __field__, __count__) \
	if (!data[#__field__].is_null()) \
	{ \
		for (auto i = 0; i < __count__; i++) \
		{ \
				vehicle->__field__[i] = data[#__field__][i].get<__type__>(); \
		} \
	} \

	namespace
	{
		constexpr const char* vehicle_fire_type_names[VEH_TURRET_FIRE_TYPE_COUNT] =
		{
			"singleFire",
			"dualFire",
			"altFire",
		};

		constexpr const char* vehicle_axle_type_names[VEH_AXLE_COUNT] =
		{
			"front",
			"rear",
			"all",
		};

		constexpr const char* vehicle_type_names[VEH_TYPE_COUNT] =
		{
			"wheels4",
			"tank",
			"plane",
			"boat",
			"artillery",
			"helicopter",
			"snowmobile",
			"submarine",
			"ugv",
			"unk9",
			"unk10",
			"mobilecover",
			"jetbike",
			"unk13",
			"hovertank",
			"divehoat",
		};

		void parse_phys_mass(VehiclePhysDef* vehicle, ordered_json& data, zone_memory* mem)
		{
			if (data.is_null())
			{
				return;
			}

#define READ_PHYSMASS_ARR(__type__, __field__, __size__) \
			for(auto idx_##__field__ = 0; idx_##__field__ < __size__; idx_##__field__++) \
			{ \
				vehicle->physMass->##__field__[idx_##__field__] = data[#__field__][idx_##__field__].get<__type__>(); \
			}

			READ_PHYSMASS_ARR(float, centerOfMass, 3);
			READ_PHYSMASS_ARR(float, momentsOfInertia, 3);
			READ_PHYSMASS_ARR(float, productsOfInertia, 3);

#undef READ_PHYSMASS_ARR
		}

		void parse_veh_phys_def(VehiclePhysDef* vehicle, ordered_json& data, zone_memory* mem)
		{
			VEHICLE_READ_FIELD(int, physicsEnabled);
			VEHICLE_READ_STRING(physPresetName);
			VEHICLE_READ_ASSET(ASSET_TYPE_PHYSPRESET, physPreset, physPreset);

			VEHICLE_READ_STRING(physMassName);
			parse_phys_mass(vehicle, data["physMass"], mem);

			VEHICLE_READ_STRING(accelGraphName);

			//VEHICLE_READ_FIELD(int, unk_48);
			//VEHICLE_READ_FIELD(int, unk_52);
			//VEHICLE_READ_FIELD(int, unk_56);
			VEHICLE_READ_FIELD(float, topSpeed);
			//VEHICLE_READ_FIELD(float, unk_64);
			VEHICLE_READ_FIELD(float, reverseSpeed);
			VEHICLE_READ_FIELD(float, maxVelocity);
			VEHICLE_READ_FIELD(float, maxPitch);
			VEHICLE_READ_FIELD(float, maxRoll);
			//VEHICLE_READ_FIELD(float, unk_84);
			VEHICLE_READ_FIELD(float, suspensionTravelFront);
			VEHICLE_READ_FIELD(float, suspensionTravelRear);
			VEHICLE_READ_FIELD(float, suspensionStrengthFront);
			VEHICLE_READ_FIELD(float, suspensionDampingFront);
			VEHICLE_READ_FIELD(float, suspensionStrengthRear);
			VEHICLE_READ_FIELD(float, suspensionDampingRear);
			VEHICLE_READ_FIELD(float, frictionBraking);
			VEHICLE_READ_FIELD(float, frictionCoasting);
			VEHICLE_READ_FIELD(float, frictionTopSpeed);
			VEHICLE_READ_FIELD(float, frictionSide);
			VEHICLE_READ_FIELD(float, frictionSideRear);
			//VEHICLE_READ_FIELD(float, unk_132);
			//VEHICLE_READ_FIELD(float, unk_136);
			//VEHICLE_READ_FIELD(float, unk_140);
			//VEHICLE_READ_FIELD(float, unk_144);
			//VEHICLE_READ_FIELD(float, unk_148);
			//VEHICLE_READ_FIELD(float, unk_152);
			//VEHICLE_READ_FIELD(float, unk_156);
			//VEHICLE_READ_FIELD(float, unk_160);
			VEHICLE_READ_FIELD(float, velocityDependentSlip);
			VEHICLE_READ_FIELD(float, rollStability);
			VEHICLE_READ_FIELD(float, rollResistance);
			VEHICLE_READ_FIELD(float, pitchResistance);
			VEHICLE_READ_FIELD(float, yawResistance);
			VEHICLE_READ_FIELD(float, uprightStrengthPitch);
			VEHICLE_READ_FIELD(float, uprightStrengthRoll);
			VEHICLE_READ_FIELD(float, targetAirPitch);
			VEHICLE_READ_FIELD(float, airYawTorque);
			VEHICLE_READ_FIELD(float, airPitchTorque);
			VEHICLE_READ_FIELD(float, minimumMomentumForCollision);
			VEHICLE_READ_FIELD(float, collisionLaunchForceScale);
			VEHICLE_READ_FIELD(float, wreckedMassScale);
			VEHICLE_READ_FIELD(float, wreckedBodyFriction);
			VEHICLE_READ_FIELD(float, minimumJoltForNotify);
			VEHICLE_READ_FIELD(float, slipThresholdFront);
			VEHICLE_READ_FIELD(float, slipThresholdRear);
			VEHICLE_READ_FIELD(float, slipFricScaleFront);
			VEHICLE_READ_FIELD(float, slipFricScaleRear);
			VEHICLE_READ_FIELD(float, slipFricRateFront);
			VEHICLE_READ_FIELD(float, slipFricRateRear);
			VEHICLE_READ_FIELD(float, slipYawTorque);
			//VEHICLE_READ_FIELD(float, unk_252);
			//VEHICLE_READ_FIELD(float, unk_256);
			//VEHICLE_READ_FIELD(float, unk_260);
		}
	}

	void vehicle_def::add_script_string(scr_string_t* ptr, const char* str)
	{
		for (std::uint32_t i = 0; i < this->script_strings.size(); i++)
		{
			if (this->script_strings[i].first == ptr)
			{
				return;
			}
		}
		this->script_strings.push_back(std::pair<scr_string_t*, const char*>(ptr, str));
	}

	const char* vehicle_def::get_script_string(scr_string_t* ptr)
	{
		for (std::uint32_t i = 0; i < this->script_strings.size(); i++)
		{
			if (this->script_strings[i].first == ptr)
			{
				return this->script_strings[i].second;
			}
		}
		return nullptr;
	}

	VehicleDef* vehicle_def::parse(const std::string& name, zone_memory* mem)
	{
		const auto path = "vehicles\\"s + name + ".json"s;

		auto file = filesystem::file(path);
		if (!file.exists())
		{
			return nullptr;
		}

		ZONETOOL_INFO("Parsing vehicle \"%s\"...", name.data());

		// parse json file
		file.open("rb");
		auto size = file.size();
		auto bytes = file.read_bytes(size);
		file.close();
		ordered_json data = json::parse(bytes);

		auto* vehicle = mem->allocate<VehicleDef>();

		// base asset
		auto base = data["baseAsset"].get<std::string>();
		VehicleDef* baseAsset = nullptr;
		if (!base.empty())
		{
			baseAsset = db_find_x_asset_header(ASSET_TYPE_VEHICLE, base.data(), 0).vehDef;
			if (baseAsset == nullptr/* || DB_IsXAssetDefault(ASSET_TYPE_VEHICLE, base.data())*/)
			{
				ZONETOOL_WARNING("Could not load base asset \"%s\" into memory...", base.data());
			}
			else
			{
				memcpy(vehicle, baseAsset, sizeof(VehicleDef));
			}
		}
		else
		{
			ZONETOOL_WARNING("No base asset is defined for vehicle \"%s\", stuff might go wrong!", name.data());
		}

		VEHICLE_READ_STRING(name);
		VEHICLE_READ_FIELD(VehicleType, type);
		VEHICLE_READ_STRING(useHintString);
		VEHICLE_READ_FIELD(int, health);
		VEHICLE_READ_FIELD(int, quadBarrel);
		//VEHICLE_READ_FIELD(int, unk_32);
		//VEHICLE_READ_FIELD(int, unk_36);
		//VEHICLE_READ_FIELD(int, unk_40);
		VEHICLE_READ_FIELD(float, texScrollScale);
		VEHICLE_READ_FIELD(float, topSpeed);
		VEHICLE_READ_FIELD(float, accel);
		VEHICLE_READ_FIELD(float, rotRate);
		VEHICLE_READ_FIELD(float, rotAccel);
		VEHICLE_READ_FIELD(float, maxBodyPitch);
		VEHICLE_READ_FIELD(float, maxBodyRoll);
		//VEHICLE_READ_FIELD(float, unk_72);
		VEHICLE_READ_FIELD(float, fakeBodyAccelPitch);
		VEHICLE_READ_FIELD(float, fakeBodyAccelRoll);
		VEHICLE_READ_FIELD(float, fakeBodyVelPitch);
		VEHICLE_READ_FIELD(float, fakeBodyVelRoll);
		VEHICLE_READ_FIELD(float, fakeBodySideVelPitch);
		VEHICLE_READ_FIELD(float, fakeBodyPitchStrength);
		VEHICLE_READ_FIELD(float, fakeBodyRollStrength);
		VEHICLE_READ_FIELD(float, fakeBodyPitchDampening);
		VEHICLE_READ_FIELD(float, fakeBodyRollDampening);
		VEHICLE_READ_FIELD(float, fakeBodyBoatRockingAmplitude);
		VEHICLE_READ_FIELD(float, fakeBodyBoatRockingPeriod);
		VEHICLE_READ_FIELD(float, fakeBodyBoatRockingRotationPeriod);
		VEHICLE_READ_FIELD(float, fakeBodyBoatRockingFadeoutSpeed);
		VEHICLE_READ_FIELD(float, boatBouncingMinForce);
		VEHICLE_READ_FIELD(float, boatBouncingMaxForce);
		VEHICLE_READ_FIELD(float, boatBouncingRate);
		VEHICLE_READ_FIELD(float, boatBouncingFadeinSpeed);
		VEHICLE_READ_FIELD(float, boatBouncingFadeoutSteeringAngle);
		VEHICLE_READ_FIELD(float, collisionDamage);
		VEHICLE_READ_FIELD(float, collisionSpeed);
		VEHICLE_READ_FIELD_ARR(float, killcamOffset, 3);
		VEHICLE_READ_FIELD(int, playerProtected);
		VEHICLE_READ_FIELD(int, bulletDamage);
		VEHICLE_READ_FIELD(int, armorPiercingDamage);
		VEHICLE_READ_FIELD(int, grenadeDamage);
		VEHICLE_READ_FIELD(int, projectileDamage);
		VEHICLE_READ_FIELD(int, projectileSplashDamage);
		VEHICLE_READ_FIELD(int, heavyExplosiveDamage);
		//VEHICLE_READ_FIELD(int, unk_196);
		
		parse_veh_phys_def(&vehicle->vehPhysDef, data["vehPhysDef"], mem);

		VEHICLE_READ_FIELD(float, boostDuration);
		VEHICLE_READ_FIELD(float, boostRechargeTime);
		VEHICLE_READ_FIELD(float, boostAcceleration);
		//VEHICLE_READ_FIELD(float, unk_476);
		VEHICLE_READ_FIELD(float, suspensionTravel);
		VEHICLE_READ_FIELD(float, maxSteeringAngle);
		VEHICLE_READ_FIELD(float, steeringLerp);
		//VEHICLE_READ_FIELD(float, steeringLerpUnk);
		VEHICLE_READ_FIELD(float, minSteeringScale);
		VEHICLE_READ_FIELD(float, minSteeringSpeed);
		//VEHICLE_READ_FIELD(float, unk_504);
		//VEHICLE_READ_FIELD(float, unk_508);

		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, effect01);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, effect02);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, effect03);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, effect04);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, effect05);

		//VEHICLE_READ_FIELD(float, unk_552);
		//VEHICLE_READ_FIELD(float, unk_556);
		//VEHICLE_READ_FIELD(float, unk_560);
		//VEHICLE_READ_FIELD(float, unk_564);
		VEHICLE_READ_FIELD(int, vehHelicopterIsASplinePlane);
		//VEHICLE_READ_FIELD(int, unk_572);
		//VEHICLE_READ_FIELD(int, unk_576);
		VEHICLE_READ_FIELD(int, vehHelicopterOffsetFromMesh);
		//VEHICLE_READ_FIELD(float, unk_584);
		//VEHICLE_READ_FIELD(float, unk_588);
		//VEHICLE_READ_FIELD(float, unk_592);
		//VEHICLE_READ_FIELD(float, unk_596);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxSpeed);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxSpeedVertical);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxAccel);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxAccelVertical);
		VEHICLE_READ_FIELD(float, vehHelicopterDecelerationFwd);
		VEHICLE_READ_FIELD(float, vehHelicopterDecelerationSide);
		VEHICLE_READ_FIELD(float, vehHelicopterDecelerationUp);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxYawRate);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxYawAccel);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromVelocity);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromControllerAxes);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromAcceleration);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromDeceleration);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltFromFwdAndYaw);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltMomentum);
		VEHICLE_READ_FIELD(float, vehHelicopterTiltSpeed);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxPitch);
		VEHICLE_READ_FIELD(float, vehHelicopterMaxRoll);
		VEHICLE_READ_FIELD(float, vehHelicopterHoverSpeedThreshold);
		VEHICLE_READ_FIELD(float, vehHelicopterJitterJerkyness);
		//VEHICLE_READ_FIELD(int, unk_684);
		//VEHICLE_READ_FIELD(float, unk_688);
		//VEHICLE_READ_FIELD(float, unk_692);
		//VEHICLE_READ_FIELD(float, unk_696);
		//VEHICLE_READ_FIELD(float, unk_700);
		//VEHICLE_READ_FIELD(float, unk_704);
		VEHICLE_READ_FIELD(float, vehHelicopterLookaheadTime);
		VEHICLE_READ_FIELD(int, vehHelicopterSoftCollisions);
		VEHICLE_READ_FIELD(int, vehHelicopterUseGroundFX);

		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, vehHelicopterGroundFx);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, vehHelicopterGroundWaterFx);

		VEHICLE_READ_FIELD(float, vehHelicopterGroundFxDefaultRepeatRate);
		VEHICLE_READ_FIELD(float, vehHelicopterGroundFxSlowestRepeatRate);
		VEHICLE_READ_FIELD(float, vehHelicopterGroundFxFastestRepeatRate);
		VEHICLE_READ_FIELD(float, vehHelicopterGroundFxMinGroundDist);
		VEHICLE_READ_FIELD(float, vehHelicopterGroundFxMaxGroundDist);
		VEHICLE_READ_FIELD(float, vehSplinePlaneCorridorMaxXVel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneCorridorMaxZVel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneCorridorMaxXAccel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneCorridorMaxZAccel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneTangentLookAtRate);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxPitchSpeed);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxYawSpeed);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxRollSpeed);
		VEHICLE_READ_FIELD(float, vehSplinePlanePitchSpeedRate);
		VEHICLE_READ_FIELD(float, vehSplinePlaneYawSpeedRate);
		VEHICLE_READ_FIELD(float, vehSplinePlaneRollSpeedRateUp);
		VEHICLE_READ_FIELD(float, vehSplinePlaneRollSpeedRateDown);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxPitchAccel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxYawAccel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxRollAccel);
		VEHICLE_READ_FIELD(float, vehSplinePlaneYawToRollFactor);
		VEHICLE_READ_FIELD(float, vehSplinePlaneRollToYawFactor);
		VEHICLE_READ_FIELD(float, vehSplinePlaneRollToYawFactorBlend);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxPitch);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxRoll);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxPitchFromRoll);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxRollYawOffset);
		VEHICLE_READ_FIELD(float, vehSplinePlaneRollYawOffsetThreshold);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxTiltRoll);
		VEHICLE_READ_FIELD(float, vehSplinePlaneMaxTiltPitch);
		VEHICLE_READ_FIELD(float, vehSplinePlaneTiltRollRate);
		VEHICLE_READ_FIELD(float, vehSplinePlaneTiltPitchRate);
		//VEHICLE_READ_FIELD(float, unk_864);
		//VEHICLE_READ_FIELD(float, unk_868);
		//VEHICLE_READ_FIELD(float, unk_872);
		//VEHICLE_READ_FIELD(float, unk_876);
		//VEHICLE_READ_FIELD(float, unk_880);
		//VEHICLE_READ_FIELD(float, unk_884);
		//VEHICLE_READ_FIELD(float, unk_888);
		//VEHICLE_READ_FIELD(float, unk_892);
		//VEHICLE_READ_FIELD(float, unk_896);
		//VEHICLE_READ_FIELD(float, unk_900);
		//VEHICLE_READ_FIELD(float, unk_904);
		//VEHICLE_READ_FIELD(float, unk_908);
		//VEHICLE_READ_FIELD(float, unk_912);
		//VEHICLE_READ_FIELD(float, unk_916);
		//VEHICLE_READ_FIELD(float, unk_920);
		//VEHICLE_READ_FIELD(float, unk_924);
		//VEHICLE_READ_FIELD(float, unk_928);
		//VEHICLE_READ_FIELD(float, unk_932);
		//VEHICLE_READ_FIELD(float, unk_936);
		//VEHICLE_READ_FIELD(float, unk_940);
		//VEHICLE_READ_FIELD(float, unk_944);
		//VEHICLE_READ_FIELD(float, unk_948);
		//VEHICLE_READ_FIELD(float, unk_952);
		//VEHICLE_READ_FIELD(float, unk_956);
		//VEHICLE_READ_FIELD(float, unk_960);
		//VEHICLE_READ_FIELD(float, unk_964);
		//VEHICLE_READ_FIELD(float, unk_968);
		//VEHICLE_READ_FIELD(float, unk_972);
		//VEHICLE_READ_FIELD(float, unk_976);
		//VEHICLE_READ_FIELD(float, unk_980);
		//VEHICLE_READ_FIELD(float, unk_984);
		//VEHICLE_READ_FIELD(float, unk_988);
		//VEHICLE_READ_FIELD(float, unk_992);
		//VEHICLE_READ_FIELD(float, unk_996);
		//VEHICLE_READ_FIELD(float, unk_1000);
		//VEHICLE_READ_FIELD(float, unk_1004);
		//VEHICLE_READ_FIELD(float, unk_1008);
		//VEHICLE_READ_FIELD(float, unk_1012);
		//VEHICLE_READ_FIELD(float, unk_1016);
		//VEHICLE_READ_FIELD(float, unk_1020);
		//VEHICLE_READ_FIELD(float, unk_1024);
		//VEHICLE_READ_FIELD(float, unk_1028);
		//VEHICLE_READ_FIELD(float, unk_1032);
		//VEHICLE_READ_FIELD(float, unk_1036);
		//VEHICLE_READ_FIELD(float, unk_1040);
		//VEHICLE_READ_FIELD(float, unk_1044);

		VEHICLE_READ_STRING(steeringGraphName);
		//VEHICLE_READ_FIELD(float, numSteeringGraphs); // runtime

		//VEHICLE_READ_FIELD(float, unk_1060);
		//VEHICLE_READ_FIELD(float, unk_1064);
		//VEHICLE_READ_FIELD(float, unk_1068);
		//VEHICLE_READ_FIELD(float, unk_1072);
		//VEHICLE_READ_FIELD(float, unk_1076);
		//VEHICLE_READ_FIELD(float, unk_1080);
		//VEHICLE_READ_FIELD(float, unk_1084);
		//VEHICLE_READ_FIELD(float, unk_1088);
		//VEHICLE_READ_FIELD(float, unk_1092);
		//VEHICLE_READ_FIELD(float, unk_1096);
		//VEHICLE_READ_FIELD(float, unk_1100);
		//VEHICLE_READ_FIELD(float, unk_1104);
		//VEHICLE_READ_FIELD(float, unk_1108);
		//VEHICLE_READ_FIELD(float, unk_1112);
		//VEHICLE_READ_FIELD(float, unk_1116);
		//VEHICLE_READ_FIELD(float, unk_1120);
		//VEHICLE_READ_FIELD(float, unk_1124);
		//VEHICLE_READ_FIELD(float, unk_1128);
		//VEHICLE_READ_FIELD(float, unk_1132);
		//VEHICLE_READ_FIELD(float, unk_1136);
		//VEHICLE_READ_FIELD(float, unk_1140);
		//VEHICLE_READ_FIELD(float, unk_1144);
		//VEHICLE_READ_FIELD(float, unk_1148);
		VEHICLE_READ_FIELD(int, camLookEnabled);
		VEHICLE_READ_FIELD(int, camRelativeControl);
		VEHICLE_READ_FIELD(int, camRemoteDrive);
		VEHICLE_READ_FIELD(float, camLerp);
		VEHICLE_READ_FIELD(float, camHeight);
		VEHICLE_READ_FIELD(float, camRadius);
		VEHICLE_READ_FIELD(float, camPitchInfluence);
		VEHICLE_READ_FIELD(float, camYawInfluence);
		VEHICLE_READ_FIELD(float, camRollInfluence);
		VEHICLE_READ_FIELD(float, camFovIncrease);
		VEHICLE_READ_FIELD(float, camFovOffset);
		VEHICLE_READ_FIELD(float, camFovSpeed);
		VEHICLE_READ_FIELD(float, camReturnSpeed);
		VEHICLE_READ_FIELD(float, camReturnLerp);
		VEHICLE_READ_FIELD(float, camVehicleAnglePitchRate);
		VEHICLE_READ_FIELD(float, camVehicleAngleYawRate);
		VEHICLE_READ_FIELD(float, camVehicleAngleRollRate);
		//VEHICLE_READ_FIELD(float, unk_1220);
		//VEHICLE_READ_FIELD(float, unk_1224);
		//VEHICLE_READ_FIELD(float, unk_1228);
		//VEHICLE_READ_FIELD(float, unk_1232);
		//VEHICLE_READ_FIELD(float, unk_1236);
		//VEHICLE_READ_FIELD(float, unk_1240);
		//VEHICLE_READ_FIELD(float, unk_1244);
		//VEHICLE_READ_FIELD(float, unk_1248);
		//VEHICLE_READ_FIELD(float, unk_1252);
		//VEHICLE_READ_FIELD(float, unk_1256);
		//VEHICLE_READ_FIELD(float, unk_1260);
		//VEHICLE_READ_FIELD(float, unk_1264);
		//VEHICLE_READ_FIELD(int, unk_1268);
		VEHICLE_READ_FIELD(float, vehCam_anglesPitch);
		VEHICLE_READ_FIELD(float, vehCam_anglesYaw);
		VEHICLE_READ_FIELD(float, vehCam_anglesRoll);
		VEHICLE_READ_FIELD(float, vehCam_offsetX);
		VEHICLE_READ_FIELD(float, vehCam_offsetY);
		VEHICLE_READ_FIELD(float, vehCam_offsetZ);
		VEHICLE_READ_FIELD(float, vehCam_radius);
		VEHICLE_READ_FIELD(float, vehCam_speedInfluence);
		VEHICLE_READ_FIELD(float, vehCam_pitchTurnRate);
		VEHICLE_READ_FIELD(float, vehCam_pitchClamp);
		VEHICLE_READ_FIELD(float, vehCam_yawTurnRate);
		VEHICLE_READ_FIELD(float, vehCam_yawClamp);
		VEHICLE_READ_FIELD(VehCamZOffsetMode, vehCam_zOffsetMode);
		VEHICLE_READ_FIELD(float, vehCam_anglesPitch3P);
		VEHICLE_READ_FIELD(float, vehCam_anglesYaw3P);
		VEHICLE_READ_FIELD(float, vehCam_anglesRoll3P);
		VEHICLE_READ_FIELD(float, vehCam_offsetX3P);
		VEHICLE_READ_FIELD(float, vehCam_offsetY3P);
		VEHICLE_READ_FIELD(float, vehCam_offsetZ3P);
		VEHICLE_READ_FIELD(float, vehCam_radius3P);
		VEHICLE_READ_FIELD(float, vehCam_speedInfluence3P);
		VEHICLE_READ_FIELD(float, vehCam_pitchTurnRate3P);
		VEHICLE_READ_FIELD(float, vehCam_pitchClamp3P);
		VEHICLE_READ_FIELD(float, vehCam_yawTurnRate3P);
		//VEHICLE_READ_FIELD(float, unk_1368);
		VEHICLE_READ_FIELD(float, vehCam_yawClamp3P);
		VEHICLE_READ_FIELD(VehCamZOffsetMode, vehCam_zOffsetMode3P);
		//VEHICLE_READ_FIELD(float, unk_1380);

		VEHICLE_READ_STRING(turretWeaponName);
		VEHICLE_READ_ASSET(ASSET_TYPE_WEAPON, weapon, turretWeapon);

		VEHICLE_READ_FIELD(float, turretHorizSpanLeft);
		VEHICLE_READ_FIELD(float, turretHorizSpanRight);
		VEHICLE_READ_FIELD(float, turretVertSpanUp);
		VEHICLE_READ_FIELD(float, turretVertSpanDown);
		VEHICLE_READ_FIELD(float, turretHorizResistLeft);
		VEHICLE_READ_FIELD(float, turretHorizResistRight);
		VEHICLE_READ_FIELD(float, turretVertResistUp);
		VEHICLE_READ_FIELD(float, turretVertResistDown);
		VEHICLE_READ_FIELD(float, turretRotRate);
		VEHICLE_READ_FIELD(VehicleTurretFireType, turretFireType);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, turretSpinSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, turretStopSnd);

		VEHICLE_READ_FIELD(int, trophyEnabled);
		VEHICLE_READ_FIELD(float, trophyRadius);
		VEHICLE_READ_FIELD(float, trophyInactiveRadius);
		VEHICLE_READ_FIELD(int, trophyAmmoCount);
		VEHICLE_READ_FIELD(float, trophyReloadTime);

		for (auto i = 0; i < 4; i++)
		{
			if (!data["trophyTags"][i].empty())
			{
				this->add_script_string(&vehicle->trophyTags[i], mem->duplicate_string(data["trophyTags"][i].get<std::string>()));
			}
		}

		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, trophyExplodeFx);
		VEHICLE_READ_ASSET(ASSET_TYPE_FX, fx, trophyFlashFx);

		VEHICLE_READ_ASSET(ASSET_TYPE_MATERIAL, material, compassFriendlyIcon);
		VEHICLE_READ_ASSET(ASSET_TYPE_MATERIAL, material, compassEnemyIcon);
		VEHICLE_READ_ASSET(ASSET_TYPE_MATERIAL, material, compassFriendlyAltIcon);
		VEHICLE_READ_ASSET(ASSET_TYPE_MATERIAL, material, compassEnemyAltIcon);

		VEHICLE_READ_FIELD(int, compassIconWidth);
		VEHICLE_READ_FIELD(int, compassIconHeight);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, idleLowSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, idleHighSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineLowSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineHighSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, sound_1584);

		VEHICLE_READ_FIELD(float, engineSndSpeed);
		if (!data["audioOriginTag"].empty())
		{
			this->add_script_string(&vehicle->audioOriginTag, mem->duplicate_string(data["audioOriginTag"].get<std::string>()));
		}

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, idleLowSndAlt);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, idleHighSndAlt);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineLowSndAlt);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineHighSndAlt);

		VEHICLE_READ_FIELD(float, engineSndSpeedAlt);
		if (!data["audioOriginTagAlt"].empty())
		{
			this->add_script_string(&vehicle->audioOriginTagAlt, mem->duplicate_string(data["audioOriginTagAlt"].get<std::string>()));
		}

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, turretSpinSndAlt);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, turretStopSndAlt);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineStartUpSnd);
		VEHICLE_READ_FIELD(int, engineStartUpLength);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineShutdownSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineIdleSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineSustainSnd);
		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineRampUpSnd);
		VEHICLE_READ_FIELD(int, engineRampUpLength);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, engineRampDownSnd);
		VEHICLE_READ_FIELD(int, engineRampDownLength);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, suspensionSoftSnd);
		VEHICLE_READ_FIELD(float, suspensionSoftCompression);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, suspensionHardSnd);
		VEHICLE_READ_FIELD(float, suspensionHardCompression);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, collisionSnd);
		VEHICLE_READ_FIELD(float, collisionBlendSpeed);

		VEHICLE_READ_ASSET(ASSET_TYPE_SOUND, sound, speedSnd);
		VEHICLE_READ_FIELD(float, speedSndBlendSpeed);

		VEHICLE_READ_STRING(surfaceSndPrefix);

		VEHICLE_READ_ASSET_ARR(ASSET_TYPE_SOUND, sound, surfaceSnds, snd_alias_list_t, 53);

		VEHICLE_READ_FIELD(float, surfaceSndBlendSpeed);
		VEHICLE_READ_FIELD(float, slideVolume);
		VEHICLE_READ_FIELD(float, slideBlendSpeed);
		VEHICLE_READ_FIELD(float, inAirPitch);

		VEHICLE_READ_STRING(soundTriggerOverrideZone);

		VEHICLE_READ_FIELD(bool, soundTriggerOverrideReverb);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideMix);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideFilter);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideOcclusion);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideAmbient);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideAmbientEvents);
		VEHICLE_READ_FIELD(bool, soundTriggerOverrideADSR);

		return vehicle;
	}

	void vehicle_def::init(const std::string& name, zone_memory* mem)
	{
		this->name_ = name;

		if (this->referenced())
		{
			this->asset_ = mem->allocate<typename std::remove_reference<decltype(*this->asset_)>::type>();
			this->asset_->name = mem->duplicate_string(name);
			return;
		}

		this->asset_ = this->parse(name, mem);
		if (!this->asset_)
		{
			ZONETOOL_FATAL("Vehicle file \"%s\" not found.", name.data());
		}
	}

	void vehicle_def::prepare(zone_buffer* buf, zone_memory* mem)
	{
		auto* vehicle = this->asset_;

		for (auto i = 0; i < 4; i++)
		{
			vehicle->trophyTags[i] = static_cast<scr_string_t>(buf->write_scriptstring(
				this->get_script_string(&vehicle->trophyTags[i])));
		}

		vehicle->audioOriginTag = static_cast<scr_string_t>(buf->write_scriptstring(
			this->get_script_string(&vehicle->audioOriginTag)));

		vehicle->audioOriginTagAlt = static_cast<scr_string_t>(buf->write_scriptstring(
			this->get_script_string(&vehicle->audioOriginTagAlt)));
	}

	void vehicle_def::load_depending(zone_base* zone)
	{
		auto vehicle = this->asset_;
#define VEHICLE_SUBASSET_DEPENDING(__field__,__type__/*,__struct__*/) \
		if (vehicle->__field__) \
		{ \
			zone->add_asset_of_type(__type__, vehicle->__field__->name); \
		}

		VEHICLE_SUBASSET_DEPENDING(vehPhysDef.physPreset, ASSET_TYPE_PHYSPRESET);

		VEHICLE_SUBASSET_DEPENDING(effect01, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(effect02, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(effect03, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(effect04, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(effect05, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(vehHelicopterGroundFx, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(vehHelicopterGroundWaterFx, ASSET_TYPE_FX);

		VEHICLE_SUBASSET_DEPENDING(turretWeapon, ASSET_TYPE_WEAPON);

		VEHICLE_SUBASSET_DEPENDING(turretSpinSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(turretStopSnd, ASSET_TYPE_SOUND);

		VEHICLE_SUBASSET_DEPENDING(trophyExplodeFx, ASSET_TYPE_FX);
		VEHICLE_SUBASSET_DEPENDING(trophyFlashFx, ASSET_TYPE_FX);

		VEHICLE_SUBASSET_DEPENDING(compassFriendlyIcon, ASSET_TYPE_MATERIAL);
		VEHICLE_SUBASSET_DEPENDING(compassEnemyIcon, ASSET_TYPE_MATERIAL);
		VEHICLE_SUBASSET_DEPENDING(compassFriendlyAltIcon, ASSET_TYPE_MATERIAL);
		VEHICLE_SUBASSET_DEPENDING(compassEnemyAltIcon, ASSET_TYPE_MATERIAL);

		VEHICLE_SUBASSET_DEPENDING(idleLowSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(idleHighSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineLowSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineHighSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(sound_1584, ASSET_TYPE_SOUND);

		VEHICLE_SUBASSET_DEPENDING(idleLowSndAlt, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(idleHighSndAlt, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineLowSndAlt, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineHighSndAlt, ASSET_TYPE_SOUND);

		VEHICLE_SUBASSET_DEPENDING(turretSpinSndAlt, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(turretStopSndAlt, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineStartUpSnd, ASSET_TYPE_SOUND);

		VEHICLE_SUBASSET_DEPENDING(engineShutdownSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineIdleSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineSustainSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineRampUpSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(engineRampDownSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(suspensionSoftSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(suspensionHardSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(collisionSnd, ASSET_TYPE_SOUND);
		VEHICLE_SUBASSET_DEPENDING(speedSnd, ASSET_TYPE_SOUND);

		for (auto i = 0; i < 53; i++)
		{
			VEHICLE_SUBASSET_DEPENDING(surfaceSnds[i], ASSET_TYPE_SOUND);
		}
	}

	std::string vehicle_def::name()
	{
		return this->name_;
	}

	std::int32_t vehicle_def::type()
	{
		return ASSET_TYPE_VEHICLE;
	}

	void vehicle_def::write(zone_base* zone, zone_buffer* buf)
	{
		auto* data = this->asset_;
		auto* dest = buf->write(data);

#define VEHICLE_STRING(__field__) \
		if (data->__field__) \
		{ \
			dest->__field__ = buf->write_str(data->__field__); \
		}

#define VEHICLE_SUBASSET(__field__,__type__,__struct__) \
		if (data->__field__) \
		{ \
			dest->__field__ = reinterpret_cast<__struct__*>(zone->get_asset_pointer(__type__, data->__field__->name)); \
		}

#define VEHICLE_SOUND_CUSTOM(__field__) \
		if (data->__field__) \
		{ \
			auto ptr = 0xFDFDFDFFFFFFFFFF; \
			buf->align(7); \
			buf->write(&ptr); \
			buf->write_str(data->__field__->name); \
			zone_buffer::clear_pointer(&dest->__field__); \
		}

		buf->push_stream(XFILE_BLOCK_VIRTUAL);

		dest->name = buf->write_str(this->name());

		VEHICLE_STRING(useHintString);

		VEHICLE_STRING(vehPhysDef.physPresetName);
		VEHICLE_SUBASSET(vehPhysDef.physPreset, ASSET_TYPE_PHYSPRESET, PhysPreset);

		VEHICLE_STRING(vehPhysDef.physMassName);
		if (data->vehPhysDef.physMass)
		{
			buf->align(3);
			buf->write(data->vehPhysDef.physMass);
			zone_buffer::clear_pointer(&dest->vehPhysDef.physMass);
		}

		VEHICLE_STRING(vehPhysDef.accelGraphName);

		VEHICLE_SUBASSET(effect01, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(effect02, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(effect03, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(effect04, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(effect05, ASSET_TYPE_FX, FxEffectDef);

		VEHICLE_SUBASSET(vehHelicopterGroundFx, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(vehHelicopterGroundWaterFx, ASSET_TYPE_FX, FxEffectDef);

		VEHICLE_STRING(steeringGraphName);
		VEHICLE_STRING(turretWeaponName);

		VEHICLE_SUBASSET(turretWeapon, ASSET_TYPE_WEAPON, WeaponDef);

		VEHICLE_SOUND_CUSTOM(turretSpinSnd);
		VEHICLE_SOUND_CUSTOM(turretStopSnd);

		VEHICLE_SUBASSET(trophyExplodeFx, ASSET_TYPE_FX, FxEffectDef);
		VEHICLE_SUBASSET(trophyFlashFx, ASSET_TYPE_FX, FxEffectDef);

		VEHICLE_SUBASSET(compassFriendlyIcon, ASSET_TYPE_MATERIAL, Material);
		VEHICLE_SUBASSET(compassEnemyIcon, ASSET_TYPE_MATERIAL, Material);
		VEHICLE_SUBASSET(compassFriendlyAltIcon, ASSET_TYPE_MATERIAL, Material);
		VEHICLE_SUBASSET(compassEnemyAltIcon, ASSET_TYPE_MATERIAL, Material);

		VEHICLE_SOUND_CUSTOM(idleLowSnd);
		VEHICLE_SOUND_CUSTOM(idleHighSnd);
		VEHICLE_SOUND_CUSTOM(engineLowSnd);
		VEHICLE_SOUND_CUSTOM(engineHighSnd);
		VEHICLE_SOUND_CUSTOM(sound_1584);
		VEHICLE_SOUND_CUSTOM(idleLowSndAlt);
		VEHICLE_SOUND_CUSTOM(idleHighSndAlt);
		VEHICLE_SOUND_CUSTOM(engineLowSndAlt);
		VEHICLE_SOUND_CUSTOM(engineHighSndAlt);
		VEHICLE_SOUND_CUSTOM(turretSpinSndAlt);
		VEHICLE_SOUND_CUSTOM(turretStopSndAlt);
		VEHICLE_SOUND_CUSTOM(engineStartUpSnd);
		VEHICLE_SOUND_CUSTOM(engineShutdownSnd);
		VEHICLE_SOUND_CUSTOM(engineIdleSnd);
		VEHICLE_SOUND_CUSTOM(engineSustainSnd);
		VEHICLE_SOUND_CUSTOM(engineRampUpSnd);
		VEHICLE_SOUND_CUSTOM(engineRampDownSnd);
		VEHICLE_SOUND_CUSTOM(suspensionSoftSnd);
		VEHICLE_SOUND_CUSTOM(suspensionHardSnd);
		VEHICLE_SOUND_CUSTOM(collisionSnd);
		VEHICLE_SOUND_CUSTOM(speedSnd);

		VEHICLE_STRING(surfaceSndPrefix);

		for (auto i = 0; i < 53; i++)
		{
			VEHICLE_SOUND_CUSTOM(surfaceSnds[i]);
		}

		VEHICLE_STRING(soundTriggerOverrideZone);

		buf->pop_stream();
	}

#define VEHICLE_DUMP_FIELD(__field__) \
	data[#__field__] = asset->__field__ \

#define VEHICLE_DUMP_SUB_FIELD(__field__, __subfield__) \
	data[#__field__][#__subfield__] = asset->__field__.__subfield__ \

#define VEHICLE_DUMP_SUB_FIELD_STRING(__field__, __subfield__) \
	if (asset->__field__.__subfield__ != nullptr) \
	{ \
		data[#__field__][#__subfield__] = asset->__field__.__subfield__; \
	} \
	else \
	{ \
		data[#__field__][#__subfield__] = nullptr; \
	} \

#define VEHICLE_DUMP_STRING(__name__) \
	if (asset->__name__ != nullptr) \
	{ \
		data[#__name__] = asset->__name__; \
	} \
	else \
	{ \
		data[#__name__] = nullptr; \
	} \

#define VEHICLE_DUMP_FIELD_ARR(__field__, __count__) \
	for (auto i = 0; i < __count__; i++) \
	{ \
		data[#__field__][i] = asset->__field__[i]; \
	} \

#define VEHICLE_DUMP_SUB_FIELD_ARR(__field__, __subfield__, __count__) \
	for (auto i = 0; i < __count__; i++) \
	{ \
		data[#__field__][#__subfield__][i] = asset->__field__.__subfield__[i]; \
	} \

#define VEHICLE_DUMP_SUB_ASSET(__field__, __subfield__) \
	if (asset->__field__.__subfield__) \
	{ \
		data[#__field__][#__subfield__] = asset->__field__.__subfield__->name; \
	} \
	else \
	{ \
		data[#__field__][#__subfield__] = ""; \
	}

#define VEHICLE_DUMP_ASSET(__field__) \
	if (asset->__field__) \
	{ \
		data[#__field__] = asset->__field__->name; \
	} \
	else \
	{ \
		data[#__field__] = ""; \
	} \

	void dump_phys_mass(VehiclePhysDef* asset, ordered_json& data)
	{
		if (!asset->physMass)
		{
			return;
		}

#define DUMP_PHYSMASS_ARR(__field__, __size__) \
			for(auto idx_##__field__ = 0; idx_##__field__ < __size__; idx_##__field__++) \
			{ \
				data[#__field__][idx_##__field__] = asset->physMass->##__field__[idx_##__field__]; \
			}

		DUMP_PHYSMASS_ARR(centerOfMass, 3);
		DUMP_PHYSMASS_ARR(momentsOfInertia, 3);
		DUMP_PHYSMASS_ARR(productsOfInertia, 3);

#undef DUMP_PHYSMASS_ARR
	}

	void dump_veh_phys_def(VehiclePhysDef* asset, ordered_json& data)
	{
		VEHICLE_DUMP_FIELD(physicsEnabled);
		VEHICLE_DUMP_STRING(physPresetName);
		VEHICLE_DUMP_ASSET(physPreset);

		VEHICLE_DUMP_STRING(physMassName);
		dump_phys_mass(asset, data["PhysMass"]);

		VEHICLE_DUMP_STRING(accelGraphName);

		//VEHICLE_DUMP_FIELD(unk_48);
		//VEHICLE_DUMP_FIELD(unk_52);
		//VEHICLE_DUMP_FIELD(unk_56);
		VEHICLE_DUMP_FIELD(topSpeed);
		//VEHICLE_DUMP_FIELD(unk_64);
		VEHICLE_DUMP_FIELD(reverseSpeed);
		VEHICLE_DUMP_FIELD(maxVelocity);
		VEHICLE_DUMP_FIELD(maxPitch);
		VEHICLE_DUMP_FIELD(maxRoll);
		//VEHICLE_DUMP_FIELD(unk_84);
		VEHICLE_DUMP_FIELD(suspensionTravelFront);
		VEHICLE_DUMP_FIELD(suspensionTravelRear);
		VEHICLE_DUMP_FIELD(suspensionStrengthFront);
		VEHICLE_DUMP_FIELD(suspensionDampingFront);
		VEHICLE_DUMP_FIELD(suspensionStrengthRear);
		VEHICLE_DUMP_FIELD(suspensionDampingRear);
		VEHICLE_DUMP_FIELD(frictionBraking);
		VEHICLE_DUMP_FIELD(frictionCoasting);
		VEHICLE_DUMP_FIELD(frictionTopSpeed);
		VEHICLE_DUMP_FIELD(frictionSide);
		VEHICLE_DUMP_FIELD(frictionSideRear);
		//VEHICLE_DUMP_FIELD(unk_132);
		//VEHICLE_DUMP_FIELD(unk_136);
		//VEHICLE_DUMP_FIELD(unk_140);
		//VEHICLE_DUMP_FIELD(unk_144);
		//VEHICLE_DUMP_FIELD(unk_148);
		//VEHICLE_DUMP_FIELD(unk_152);
		//VEHICLE_DUMP_FIELD(unk_156);
		//VEHICLE_DUMP_FIELD(unk_160);
		VEHICLE_DUMP_FIELD(velocityDependentSlip);
		VEHICLE_DUMP_FIELD(rollStability);
		VEHICLE_DUMP_FIELD(rollResistance);
		VEHICLE_DUMP_FIELD(pitchResistance);
		VEHICLE_DUMP_FIELD(yawResistance);
		VEHICLE_DUMP_FIELD(uprightStrengthPitch);
		VEHICLE_DUMP_FIELD(uprightStrengthRoll);
		VEHICLE_DUMP_FIELD(targetAirPitch);
		VEHICLE_DUMP_FIELD(airYawTorque);
		VEHICLE_DUMP_FIELD(airPitchTorque);
		VEHICLE_DUMP_FIELD(minimumMomentumForCollision);
		VEHICLE_DUMP_FIELD(collisionLaunchForceScale);
		VEHICLE_DUMP_FIELD(wreckedMassScale);
		VEHICLE_DUMP_FIELD(wreckedBodyFriction);
		VEHICLE_DUMP_FIELD(minimumJoltForNotify);
		VEHICLE_DUMP_FIELD(slipThresholdFront);
		VEHICLE_DUMP_FIELD(slipThresholdRear);
		VEHICLE_DUMP_FIELD(slipFricScaleFront);
		VEHICLE_DUMP_FIELD(slipFricScaleRear);
		VEHICLE_DUMP_FIELD(slipFricRateFront);
		VEHICLE_DUMP_FIELD(slipFricRateRear);
		VEHICLE_DUMP_FIELD(slipYawTorque);
		//VEHICLE_DUMP_FIELD(unk_252);
		//VEHICLE_DUMP_FIELD(unk_256);
		//VEHICLE_DUMP_FIELD(unk_260);
	}

	void vehicle_def::dump(VehicleDef* asset)
	{
		const auto path = "vehicles\\"s + asset->name + ".json"s;

		ordered_json data;

		data["baseAsset"] = asset->name;

		VEHICLE_DUMP_FIELD(name);
		VEHICLE_DUMP_FIELD(type);
		VEHICLE_DUMP_STRING(useHintString);
		VEHICLE_DUMP_FIELD(health);
		VEHICLE_DUMP_FIELD(quadBarrel);
		//VEHICLE_DUMP_FIELD(unk_32);
		//VEHICLE_DUMP_FIELD(unk_36);
		//VEHICLE_DUMP_FIELD(unk_40);
		VEHICLE_DUMP_FIELD(texScrollScale);
		VEHICLE_DUMP_FIELD(topSpeed);
		VEHICLE_DUMP_FIELD(accel);
		VEHICLE_DUMP_FIELD(rotRate);
		VEHICLE_DUMP_FIELD(rotAccel);
		VEHICLE_DUMP_FIELD(maxBodyPitch);
		VEHICLE_DUMP_FIELD(maxBodyRoll);
		//VEHICLE_DUMP_FIELD(unk_72);
		VEHICLE_DUMP_FIELD(fakeBodyAccelPitch);
		VEHICLE_DUMP_FIELD(fakeBodyAccelRoll);
		VEHICLE_DUMP_FIELD(fakeBodyVelPitch);
		VEHICLE_DUMP_FIELD(fakeBodyVelRoll);
		VEHICLE_DUMP_FIELD(fakeBodySideVelPitch);
		VEHICLE_DUMP_FIELD(fakeBodyPitchStrength);
		VEHICLE_DUMP_FIELD(fakeBodyRollStrength);
		VEHICLE_DUMP_FIELD(fakeBodyPitchDampening);
		VEHICLE_DUMP_FIELD(fakeBodyRollDampening);
		VEHICLE_DUMP_FIELD(fakeBodyBoatRockingAmplitude);
		VEHICLE_DUMP_FIELD(fakeBodyBoatRockingPeriod);
		VEHICLE_DUMP_FIELD(fakeBodyBoatRockingRotationPeriod);
		VEHICLE_DUMP_FIELD(fakeBodyBoatRockingFadeoutSpeed);
		VEHICLE_DUMP_FIELD(boatBouncingMinForce);
		VEHICLE_DUMP_FIELD(boatBouncingMaxForce);
		VEHICLE_DUMP_FIELD(boatBouncingRate);
		VEHICLE_DUMP_FIELD(boatBouncingFadeinSpeed);
		VEHICLE_DUMP_FIELD(boatBouncingFadeoutSteeringAngle);
		VEHICLE_DUMP_FIELD(collisionDamage);
		VEHICLE_DUMP_FIELD(collisionSpeed);
		VEHICLE_DUMP_FIELD_ARR(killcamOffset, 3);
		VEHICLE_DUMP_FIELD(playerProtected);
		VEHICLE_DUMP_FIELD(bulletDamage);
		VEHICLE_DUMP_FIELD(armorPiercingDamage);
		VEHICLE_DUMP_FIELD(grenadeDamage);
		VEHICLE_DUMP_FIELD(projectileDamage);
		VEHICLE_DUMP_FIELD(projectileSplashDamage);
		VEHICLE_DUMP_FIELD(heavyExplosiveDamage);
		//VEHICLE_DUMP_FIELD(unk_196);

		dump_veh_phys_def(&asset->vehPhysDef, data["vehPhysDef"]);

		VEHICLE_DUMP_FIELD(boostDuration);
		VEHICLE_DUMP_FIELD(boostRechargeTime);
		VEHICLE_DUMP_FIELD(boostAcceleration);
		//VEHICLE_DUMP_FIELD(unk_476);
		VEHICLE_DUMP_FIELD(suspensionTravel);
		VEHICLE_DUMP_FIELD(maxSteeringAngle);
		VEHICLE_DUMP_FIELD(steeringLerp);
		//VEHICLE_DUMP_FIELD(steeringLerpUnk);
		VEHICLE_DUMP_FIELD(minSteeringScale);
		VEHICLE_DUMP_FIELD(minSteeringSpeed);
		//VEHICLE_DUMP_FIELD(unk_504);
		//VEHICLE_DUMP_FIELD(unk_508);

		VEHICLE_DUMP_ASSET(effect01);
		VEHICLE_DUMP_ASSET(effect02);
		VEHICLE_DUMP_ASSET(effect03);
		VEHICLE_DUMP_ASSET(effect04);
		VEHICLE_DUMP_ASSET(effect05);

		//VEHICLE_DUMP_FIELD(unk_552);
		//VEHICLE_DUMP_FIELD(unk_556);
		//VEHICLE_DUMP_FIELD(unk_560);
		//VEHICLE_DUMP_FIELD(unk_564);
		VEHICLE_DUMP_FIELD(vehHelicopterIsASplinePlane);
		//VEHICLE_DUMP_FIELD(unk_572);
		//VEHICLE_DUMP_FIELD(unk_576);
		VEHICLE_DUMP_FIELD(vehHelicopterOffsetFromMesh);
		//VEHICLE_DUMP_FIELD(unk_584);
		//VEHICLE_DUMP_FIELD(unk_588);
		//VEHICLE_DUMP_FIELD(unk_592);
		//VEHICLE_DUMP_FIELD(unk_596);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxSpeed);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxSpeedVertical);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxAccel);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxAccelVertical);
		VEHICLE_DUMP_FIELD(vehHelicopterDecelerationFwd);
		VEHICLE_DUMP_FIELD(vehHelicopterDecelerationSide);
		VEHICLE_DUMP_FIELD(vehHelicopterDecelerationUp);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxYawRate);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxYawAccel);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromVelocity);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromControllerAxes);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromAcceleration);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromDeceleration);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltFromFwdAndYaw);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltMomentum);
		VEHICLE_DUMP_FIELD(vehHelicopterTiltSpeed);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxPitch);
		VEHICLE_DUMP_FIELD(vehHelicopterMaxRoll);
		VEHICLE_DUMP_FIELD(vehHelicopterHoverSpeedThreshold);
		VEHICLE_DUMP_FIELD(vehHelicopterJitterJerkyness);
		//VEHICLE_DUMP_FIELD(unk_684);
		//VEHICLE_DUMP_FIELD(unk_688);
		//VEHICLE_DUMP_FIELD(unk_692);
		//VEHICLE_DUMP_FIELD(unk_696);
		//VEHICLE_DUMP_FIELD(unk_700);
		//VEHICLE_DUMP_FIELD(unk_704);
		VEHICLE_DUMP_FIELD(vehHelicopterLookaheadTime);
		VEHICLE_DUMP_FIELD(vehHelicopterSoftCollisions);
		VEHICLE_DUMP_FIELD(vehHelicopterUseGroundFX);

		VEHICLE_DUMP_ASSET(vehHelicopterGroundFx);
		VEHICLE_DUMP_ASSET(vehHelicopterGroundWaterFx);

		VEHICLE_DUMP_FIELD(vehHelicopterGroundFxDefaultRepeatRate);
		VEHICLE_DUMP_FIELD(vehHelicopterGroundFxSlowestRepeatRate);
		VEHICLE_DUMP_FIELD(vehHelicopterGroundFxFastestRepeatRate);
		VEHICLE_DUMP_FIELD(vehHelicopterGroundFxMinGroundDist);
		VEHICLE_DUMP_FIELD(vehHelicopterGroundFxMaxGroundDist);
		VEHICLE_DUMP_FIELD(vehSplinePlaneCorridorMaxXVel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneCorridorMaxZVel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneCorridorMaxXAccel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneCorridorMaxZAccel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneTangentLookAtRate);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxPitchSpeed);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxYawSpeed);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxRollSpeed);
		VEHICLE_DUMP_FIELD(vehSplinePlanePitchSpeedRate);
		VEHICLE_DUMP_FIELD(vehSplinePlaneYawSpeedRate);
		VEHICLE_DUMP_FIELD(vehSplinePlaneRollSpeedRateUp);
		VEHICLE_DUMP_FIELD(vehSplinePlaneRollSpeedRateDown);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxPitchAccel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxYawAccel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxRollAccel);
		VEHICLE_DUMP_FIELD(vehSplinePlaneYawToRollFactor);
		VEHICLE_DUMP_FIELD(vehSplinePlaneRollToYawFactor);
		VEHICLE_DUMP_FIELD(vehSplinePlaneRollToYawFactorBlend);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxPitch);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxRoll);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxPitchFromRoll);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxRollYawOffset);
		VEHICLE_DUMP_FIELD(vehSplinePlaneRollYawOffsetThreshold);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxTiltRoll);
		VEHICLE_DUMP_FIELD(vehSplinePlaneMaxTiltPitch);
		VEHICLE_DUMP_FIELD(vehSplinePlaneTiltRollRate);
		VEHICLE_DUMP_FIELD(vehSplinePlaneTiltPitchRate);
		//VEHICLE_DUMP_FIELD(unk_864);
		//VEHICLE_DUMP_FIELD(unk_868);
		//VEHICLE_DUMP_FIELD(unk_872);
		//VEHICLE_DUMP_FIELD(unk_876);
		//VEHICLE_DUMP_FIELD(unk_880);
		//VEHICLE_DUMP_FIELD(unk_884);
		//VEHICLE_DUMP_FIELD(unk_888);
		//VEHICLE_DUMP_FIELD(unk_892);
		//VEHICLE_DUMP_FIELD(unk_896);
		//VEHICLE_DUMP_FIELD(unk_900);
		//VEHICLE_DUMP_FIELD(unk_904);
		//VEHICLE_DUMP_FIELD(unk_908);
		//VEHICLE_DUMP_FIELD(unk_912);
		//VEHICLE_DUMP_FIELD(unk_916);
		//VEHICLE_DUMP_FIELD(unk_920);
		//VEHICLE_DUMP_FIELD(unk_924);
		//VEHICLE_DUMP_FIELD(unk_928);
		//VEHICLE_DUMP_FIELD(unk_932);
		//VEHICLE_DUMP_FIELD(unk_936);
		//VEHICLE_DUMP_FIELD(unk_940);
		//VEHICLE_DUMP_FIELD(unk_944);
		//VEHICLE_DUMP_FIELD(unk_948);
		//VEHICLE_DUMP_FIELD(unk_952);
		//VEHICLE_DUMP_FIELD(unk_956);
		//VEHICLE_DUMP_FIELD(unk_960);
		//VEHICLE_DUMP_FIELD(unk_964);
		//VEHICLE_DUMP_FIELD(unk_968);
		//VEHICLE_DUMP_FIELD(unk_972);
		//VEHICLE_DUMP_FIELD(unk_976);
		//VEHICLE_DUMP_FIELD(unk_980);
		//VEHICLE_DUMP_FIELD(unk_984);
		//VEHICLE_DUMP_FIELD(unk_988);
		//VEHICLE_DUMP_FIELD(unk_992);
		//VEHICLE_DUMP_FIELD(unk_996);
		//VEHICLE_DUMP_FIELD(unk_1000);
		//VEHICLE_DUMP_FIELD(unk_1004);
		//VEHICLE_DUMP_FIELD(unk_1008);
		//VEHICLE_DUMP_FIELD(unk_1012);
		//VEHICLE_DUMP_FIELD(unk_1016);
		//VEHICLE_DUMP_FIELD(unk_1020);
		//VEHICLE_DUMP_FIELD(unk_1024);
		//VEHICLE_DUMP_FIELD(unk_1028);
		//VEHICLE_DUMP_FIELD(unk_1032);
		//VEHICLE_DUMP_FIELD(unk_1036);
		//VEHICLE_DUMP_FIELD(unk_1040);
		//VEHICLE_DUMP_FIELD(unk_1044);

		VEHICLE_DUMP_STRING(steeringGraphName);
		//VEHICLE_DUMP_FIELD(numSteeringGraphs); // runtime

		//VEHICLE_DUMP_FIELD(unk_1060);
		//VEHICLE_DUMP_FIELD(unk_1064);
		//VEHICLE_DUMP_FIELD(unk_1068);
		//VEHICLE_DUMP_FIELD(unk_1072);
		//VEHICLE_DUMP_FIELD(unk_1076);
		//VEHICLE_DUMP_FIELD(unk_1080);
		//VEHICLE_DUMP_FIELD(unk_1084);
		//VEHICLE_DUMP_FIELD(unk_1088);
		//VEHICLE_DUMP_FIELD(unk_1092);
		//VEHICLE_DUMP_FIELD(unk_1096);
		//VEHICLE_DUMP_FIELD(unk_1100);
		//VEHICLE_DUMP_FIELD(unk_1104);
		//VEHICLE_DUMP_FIELD(unk_1108);
		//VEHICLE_DUMP_FIELD(unk_1112);
		//VEHICLE_DUMP_FIELD(unk_1116);
		//VEHICLE_DUMP_FIELD(unk_1120);
		//VEHICLE_DUMP_FIELD(unk_1124);
		//VEHICLE_DUMP_FIELD(unk_1128);
		//VEHICLE_DUMP_FIELD(unk_1132);
		//VEHICLE_DUMP_FIELD(unk_1136);
		//VEHICLE_DUMP_FIELD(unk_1140);
		//VEHICLE_DUMP_FIELD(unk_1144);
		//VEHICLE_DUMP_FIELD(unk_1148);
		VEHICLE_DUMP_FIELD(camLookEnabled);
		VEHICLE_DUMP_FIELD(camRelativeControl);
		VEHICLE_DUMP_FIELD(camRemoteDrive);
		VEHICLE_DUMP_FIELD(camLerp);
		VEHICLE_DUMP_FIELD(camHeight);
		VEHICLE_DUMP_FIELD(camRadius);
		VEHICLE_DUMP_FIELD(camPitchInfluence);
		VEHICLE_DUMP_FIELD(camYawInfluence);
		VEHICLE_DUMP_FIELD(camRollInfluence);
		VEHICLE_DUMP_FIELD(camFovIncrease);
		VEHICLE_DUMP_FIELD(camFovOffset);
		VEHICLE_DUMP_FIELD(camFovSpeed);
		VEHICLE_DUMP_FIELD(camReturnSpeed);
		VEHICLE_DUMP_FIELD(camReturnLerp);
		VEHICLE_DUMP_FIELD(camVehicleAnglePitchRate);
		VEHICLE_DUMP_FIELD(camVehicleAngleYawRate);
		VEHICLE_DUMP_FIELD(camVehicleAngleRollRate);
		//VEHICLE_DUMP_FIELD(unk_1220);
		//VEHICLE_DUMP_FIELD(unk_1224);
		//VEHICLE_DUMP_FIELD(unk_1228);
		//VEHICLE_DUMP_FIELD(unk_1232);
		//VEHICLE_DUMP_FIELD(unk_1236);
		//VEHICLE_DUMP_FIELD(unk_1240);
		//VEHICLE_DUMP_FIELD(unk_1244);
		//VEHICLE_DUMP_FIELD(unk_1248);
		//VEHICLE_DUMP_FIELD(unk_1252);
		//VEHICLE_DUMP_FIELD(unk_1256);
		//VEHICLE_DUMP_FIELD(unk_1260);
		//VEHICLE_DUMP_FIELD(unk_1264);
		//VEHICLE_DUMP_FIELD(unk_1268);
		VEHICLE_DUMP_FIELD(vehCam_anglesPitch);
		VEHICLE_DUMP_FIELD(vehCam_anglesYaw);
		VEHICLE_DUMP_FIELD(vehCam_anglesRoll);
		VEHICLE_DUMP_FIELD(vehCam_offsetX);
		VEHICLE_DUMP_FIELD(vehCam_offsetY);
		VEHICLE_DUMP_FIELD(vehCam_offsetZ);
		VEHICLE_DUMP_FIELD(vehCam_radius);
		VEHICLE_DUMP_FIELD(vehCam_speedInfluence);
		VEHICLE_DUMP_FIELD(vehCam_pitchTurnRate);
		VEHICLE_DUMP_FIELD(vehCam_pitchClamp);
		VEHICLE_DUMP_FIELD(vehCam_yawTurnRate);
		VEHICLE_DUMP_FIELD(vehCam_yawClamp);
		VEHICLE_DUMP_FIELD(vehCam_zOffsetMode);
		VEHICLE_DUMP_FIELD(vehCam_anglesPitch3P);
		VEHICLE_DUMP_FIELD(vehCam_anglesYaw3P);
		VEHICLE_DUMP_FIELD(vehCam_anglesRoll3P);
		VEHICLE_DUMP_FIELD(vehCam_offsetX3P);
		VEHICLE_DUMP_FIELD(vehCam_offsetY3P);
		VEHICLE_DUMP_FIELD(vehCam_offsetZ3P);
		VEHICLE_DUMP_FIELD(vehCam_radius3P);
		VEHICLE_DUMP_FIELD(vehCam_speedInfluence3P);
		VEHICLE_DUMP_FIELD(vehCam_pitchTurnRate3P);
		VEHICLE_DUMP_FIELD(vehCam_pitchClamp3P);
		VEHICLE_DUMP_FIELD(vehCam_yawTurnRate3P);
		//VEHICLE_DUMP_FIELD(unk_1368);
		VEHICLE_DUMP_FIELD(vehCam_yawClamp3P);
		VEHICLE_DUMP_FIELD(vehCam_zOffsetMode3P);
		//VEHICLE_DUMP_FIELD(unk_1380);

		VEHICLE_DUMP_STRING(turretWeaponName);
		VEHICLE_DUMP_ASSET(turretWeapon);
		VEHICLE_DUMP_FIELD(turretHorizSpanLeft);
		VEHICLE_DUMP_FIELD(turretHorizSpanRight);
		VEHICLE_DUMP_FIELD(turretVertSpanUp);
		VEHICLE_DUMP_FIELD(turretVertSpanDown);
		VEHICLE_DUMP_FIELD(turretHorizResistLeft);
		VEHICLE_DUMP_FIELD(turretHorizResistRight);
		VEHICLE_DUMP_FIELD(turretVertResistUp);
		VEHICLE_DUMP_FIELD(turretVertResistDown);
		VEHICLE_DUMP_FIELD(turretRotRate);
		VEHICLE_DUMP_FIELD(turretFireType);
		VEHICLE_DUMP_ASSET(turretSpinSnd);
		VEHICLE_DUMP_FIELD(trophyEnabled);
		VEHICLE_DUMP_FIELD(trophyEnabled);
		VEHICLE_DUMP_FIELD(trophyRadius);
		VEHICLE_DUMP_FIELD(trophyInactiveRadius);
		VEHICLE_DUMP_FIELD(trophyAmmoCount);
		VEHICLE_DUMP_FIELD(trophyReloadTime);

		for (auto i = 0; i < 4; i++)
		{
			if (asset->trophyTags[i])
			{
				data["trophyTags"][i] = SL_ConvertToString(asset->trophyTags[i]);
			}
			else
			{
				data["trophyTags"][i] = "";
			}
		}

		VEHICLE_DUMP_ASSET(trophyExplodeFx);
		VEHICLE_DUMP_ASSET(trophyFlashFx);

		VEHICLE_DUMP_ASSET(compassFriendlyIcon);
		VEHICLE_DUMP_ASSET(compassEnemyIcon);
		VEHICLE_DUMP_ASSET(compassFriendlyAltIcon);
		VEHICLE_DUMP_ASSET(compassEnemyAltIcon);

		VEHICLE_DUMP_FIELD(compassIconWidth);
		VEHICLE_DUMP_FIELD(compassIconHeight);
		VEHICLE_DUMP_ASSET(idleLowSnd);
		VEHICLE_DUMP_ASSET(idleHighSnd);
		VEHICLE_DUMP_ASSET(engineLowSnd);
		VEHICLE_DUMP_ASSET(engineHighSnd);
		VEHICLE_DUMP_ASSET(sound_1584);
		VEHICLE_DUMP_FIELD(engineSndSpeed);

		if (asset->audioOriginTag)
		{
			data["audioOriginTag"] = SL_ConvertToString(asset->audioOriginTag);
		}
		else
		{
			data["audioOriginTag"] = "";
		}

		VEHICLE_DUMP_ASSET(idleLowSndAlt);
		VEHICLE_DUMP_ASSET(idleHighSndAlt);
		VEHICLE_DUMP_ASSET(engineLowSndAlt);
		VEHICLE_DUMP_ASSET(engineHighSndAlt);
		VEHICLE_DUMP_FIELD(engineSndSpeedAlt);

		if (asset->audioOriginTagAlt)
		{
			data["audioOriginTagAlt"] = SL_ConvertToString(asset->audioOriginTagAlt);
		}
		else
		{
			data["audioOriginTagAlt"] = "";
		}

		VEHICLE_DUMP_ASSET(turretSpinSndAlt);
		VEHICLE_DUMP_ASSET(turretStopSndAlt);
		VEHICLE_DUMP_ASSET(engineStartUpSnd);
		VEHICLE_DUMP_FIELD(engineStartUpLength);
		VEHICLE_DUMP_ASSET(engineShutdownSnd);
		VEHICLE_DUMP_ASSET(engineIdleSnd);
		VEHICLE_DUMP_ASSET(engineSustainSnd);
		VEHICLE_DUMP_ASSET(engineRampUpSnd);
		VEHICLE_DUMP_FIELD(engineRampUpLength);
		VEHICLE_DUMP_ASSET(engineRampDownSnd);
		VEHICLE_DUMP_FIELD(engineRampDownLength);
		VEHICLE_DUMP_ASSET(suspensionSoftSnd);
		VEHICLE_DUMP_FIELD(suspensionSoftCompression);
		VEHICLE_DUMP_ASSET(suspensionHardSnd);
		VEHICLE_DUMP_FIELD(suspensionHardCompression);
		VEHICLE_DUMP_ASSET(collisionSnd);
		VEHICLE_DUMP_FIELD(collisionBlendSpeed);
		VEHICLE_DUMP_ASSET(speedSnd);
		VEHICLE_DUMP_FIELD(speedSndBlendSpeed);
		VEHICLE_DUMP_STRING(surfaceSndPrefix);

		for (auto i = 0; i < 53; i++)
		{
			if (asset->surfaceSnds[i])
			{
				data["surfaceSnds"][i] = asset->surfaceSnds[i]->name;
			}
			else
			{
				data["surfaceSnds"][i] = "";
			}
		}

		VEHICLE_DUMP_FIELD(surfaceSndBlendSpeed);
		VEHICLE_DUMP_FIELD(slideVolume);
		VEHICLE_DUMP_FIELD(slideBlendSpeed);
		VEHICLE_DUMP_FIELD(inAirPitch);
		VEHICLE_DUMP_STRING(soundTriggerOverrideZone);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideReverb);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideMix);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideFilter);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideOcclusion);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideAmbient);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideAmbientEvents);
		VEHICLE_DUMP_FIELD(soundTriggerOverrideADSR);

		std::string json = data.dump(4);

		auto file = filesystem::file(path);
		file.open("wb");
		file.write(json.data(), json.size(), 1);
		file.close();
	}
}