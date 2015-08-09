#include "stdafx.h"
#include "MySensorsBase.h"
#include "../main/Logger.h"
#include "../main/Helper.h"
#include "../main/RFXtrx.h"
#include "../main/SQLHelper.h"
#include "../main/localtime_r.h"
#include "hardwaretypes.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

#include <ctime>

#define round(a) ( int ) ( a + .5 )

const MySensorsBase::_tMySensorsReverseTypeLookup MySensorsBase::m_MySenserReverseValueTable[] =
{
	{ V_TEMP, "V_TEMP" },
	{ V_HUM, "V_HUM" },
	{ V_LIGHT, "V_LIGHT" },
	{ V_DIMMER, "V_DIMMER" },
	{ V_PRESSURE, "V_PRESSURE" },
	{ V_FORECAST, "V_FORECAST" },
	{ V_RAIN, "V_RAIN" },
	{ V_RAINRATE, "V_RAINRATE" },
	{ V_WIND, "V_WIND" },
	{ V_GUST, "V_GUST" },
	{ V_DIRECTION, "V_DIRECTION" },
	{ V_UV, "V_UV" },
	{ V_WEIGHT, "V_WEIGHT" },
	{ V_DISTANCE, "V_DISTANCE" },
	{ V_IMPEDANCE, "V_IMPEDANCE" },
	{ V_ARMED, "V_ARMED" },
	{ V_TRIPPED, "V_TRIPPED" },
	{ V_WATT, "V_WATT" },
	{ V_KWH, "V_KWH" },
	{ V_SCENE_ON, "V_SCENE_ON" },
	{ V_SCENE_OFF, "V_SCENE_OFF" },
	{ V_HEATER, "V_HEATER" },
	{ V_HEATER_SW, "V_HEATER_SW" },
	{ V_LIGHT_LEVEL, "V_LIGHT_LEVEL" },
	{ V_VAR1, "V_VAR1" },
	{ V_VAR2, "V_VAR2" },
	{ V_VAR3, "V_VAR3" },
	{ V_VAR4, "V_VAR4" },
	{ V_VAR5, "V_VAR5" },
	{ V_UP, "V_UP" },
	{ V_DOWN, "V_DOWN" },
	{ V_STOP, "V_STOP" },
	{ V_IR_SEND, "V_IR_SEND" },
	{ V_IR_RECEIVE, "V_IR_RECEIVE" },
	{ V_FLOW, "V_FLOW" },
	{ V_VOLUME, "V_VOLUME" },
	{ V_LOCK_STATUS, "V_LOCK_STATUS" },
	{ V_DUST_LEVEL, "V_DUST_LEVEL" },
	{ V_VOLTAGE, "V_VOLTAGE" },
	{ V_CURRENT, "V_CURRENT" },
	{ V_RGB, "V_RGB" },
	{ V_RGBW, "V_RGBW" },
	{ V_ID, "V_ID" },
	{ V_UNIT_PREFIX, "V_UNIT_PREFIX" },
	{ V_HVAC_SETPOINT_COOL, "V_HVAC_SETPOINT_COOL" },
	{ V_HVAC_SETPOINT_HEAT, "V_HVAC_SETPOINT_HEAT" },
	{ V_HVAC_FLOW_MODE, "V_HVAC_FLOW_MODE" },
	{ 0, NULL }
};

bool MySensorsBase::GetReverseValueLookup(const std::string &ValueString, _eSetType &retSetType)
{
	const _tMySensorsReverseTypeLookup *pTable = (const _tMySensorsReverseTypeLookup *)&m_MySenserReverseValueTable;
	while (pTable->stringType != NULL)
	{
		if (pTable->stringType == ValueString)
		{
			retSetType = (_eSetType)pTable->SType;
			return true;
		}
		pTable++;
	}
	return false;
}

const MySensorsBase::_tMySensorsReverseTypeLookup MySensorsBase::m_MySenserReversePresentationTable[] =
{
	{ S_DOOR, "S_DOOR" },
	{ S_MOTION, "S_MOTION" },
	{ S_SMOKE, "S_SMOKE" },
	{ S_LIGHT, "S_LIGHT" },
	{ S_DIMMER, "S_DIMMER" },
	{ S_COVER, "S_COVER" },
	{ S_TEMP, "S_TEMP" },
	{ S_HUM, "S_HUM" },
	{ S_BARO, "S_BARO" },
	{ S_WIND, "S_WIND" },
	{ S_RAIN, "S_RAIN" },
	{ S_UV, "S_UV" },
	{ S_WEIGHT, "S_WEIGHT" },
	{ S_POWER, "S_POWER" },
	{ S_HEATER, "S_HEATER" },
	{ S_DISTANCE, "S_DISTANCE" },
	{ S_LIGHT_LEVEL, "S_LIGHT_LEVEL" },
	{ S_ARDUINO_NODE, "S_ARDUINO_NODE" },
	{ S_ARDUINO_RELAY, "S_ARDUINO_RELAY" },
	{ S_LOCK, "S_LOCK" },
	{ S_IR, "S_IR" },
	{ S_WATER, "S_WATER" },
	{ S_AIR_QUALITY, "S_AIR_QUALITY" },
	{ S_CUSTOM, "S_CUSTOM" },
	{ S_DUST, "S_DUST" },
	{ S_SCENE_CONTROLLER, "S_SCENE_CONTROLLER" },
	{ S_RGB_LIGHT, "S_RGB_LIGHT" },
	{ S_RGBW_LIGHT, "S_RGBW_LIGHT" },
	{ S_COLOR_SENSOR, "S_COLOR_SENSOR" },
	{ S_HVAC, "S_HVAC" },
	{ S_MULTIMETER, "S_MULTIMETER" },
	{ S_SPRINKLER, "S_SPRINKLER" },
	{ S_WATER_LEAK, "S_WATER_LEAK" },
	{ S_SOUND, "S_SOUND" },
	{ S_VIBRATION, "S_VIBRATION" },
	{ S_MOISTURE, "S_MOISTURE" },
	{ 0, NULL }
};

bool MySensorsBase::GetReversePresentationLookup(const std::string &ValueString, _ePresentationType &retSetType)
{
	const _tMySensorsReverseTypeLookup *pTable = (const _tMySensorsReverseTypeLookup *)&m_MySenserReversePresentationTable;
	while (pTable->stringType != NULL)
	{
		if (pTable->stringType == ValueString)
		{
			retSetType = (_ePresentationType)pTable->SType;
			return true;
		}
		pTable++;
	}
	return false;
}


const MySensorsBase::_tMySensorsReverseTypeLookup MySensorsBase::m_MySenserReverseTypeTable[] =
{
	{ MT_Presentation, "Presentation" },
	{ MT_Set, "Set" },
	{ MT_Req, "Req" },
	{ MT_Internal, "Internal" },
	{ MT_Stream, "Stream" },
	{ MT_Presentation, NULL }
};

bool MySensorsBase::GetReverseTypeLookup(const std::string &ValueString, _eMessageType &retSetType)
{
	const _tMySensorsReverseTypeLookup *pTable = (const _tMySensorsReverseTypeLookup *)&m_MySenserReverseTypeTable;
	while (pTable->stringType != NULL)
	{
		if (pTable->stringType == ValueString)
		{
			retSetType = (_eMessageType)pTable->SType;
			return true;
		}
		pTable++;
	}
	return false;
}

MySensorsBase::MySensorsBase(void)
{
	m_bufferpos = 0;
}


MySensorsBase::~MySensorsBase(void)
{
}

void MySensorsBase::LoadDevicesFromDatabase()
{
	boost::lock_guard<boost::mutex> l(readQueueMutex);
	m_nodes.clear();

	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT ID, SketchName, SketchVersion FROM MySensors WHERE (HardwareID=%d) ORDER BY ID ASC", m_HwdID);
	if (result.size() > 0)
	{
		std::vector<std::vector<std::string> >::const_iterator itt;
		for (itt = result.begin(); itt != result.end(); ++itt)
		{
			std::vector<std::string> sd = *itt;

			int ID = atoi(sd[0].c_str());
			std::string SkectName = sd[1];
			std::string SkectVersion = sd[2];

			_tMySensorNode mNode;
			mNode.nodeID = ID;
			mNode.SketchName = SkectName;
			mNode.SketchVersion = SkectVersion;
			mNode.lastreceived = mytime(NULL);
			m_nodes[ID] = mNode;
		}
	}
}

void MySensorsBase::Add2Database(const int nodeID, const std::string &SketchName, const std::string &SketchVersion)
{
	m_sql.safe_query("INSERT INTO MySensors (HardwareID, ID, SketchName, SketchVersion) VALUES (%d,%d, '%q', '%q')", m_HwdID, nodeID, SketchName.c_str(), SketchVersion.c_str());
}

void MySensorsBase::DatabaseUpdateSketchName(const int nodeID, const std::string &SketchName)
{
	m_sql.safe_query("UPDATE MySensors SET SketchName='%q' WHERE (HardwareID=%d) AND (ID=%d)", SketchName.c_str(), m_HwdID, nodeID);
}

void MySensorsBase::DatabaseUpdateSketchVersion(const int nodeID, const std::string &SketchVersion)
{
	m_sql.safe_query("UPDATE MySensors SET SketchVersion='%q' WHERE (HardwareID=%d) AND (ID=%d)", SketchVersion.c_str(), m_HwdID, nodeID);
}

int MySensorsBase::FindNextNodeID()
{
	unsigned char _UsedValues[256];
	memset(_UsedValues, 0, sizeof(_UsedValues));
	std::map<int, _tMySensorNode>::const_iterator itt;
	for (itt = m_nodes.begin(); itt != m_nodes.end(); ++itt)
	{
		int ID = itt->first;
		if (ID < 255)
		{
			_UsedValues[ID] = 1;
		}
	}
	for (int ii = 1; ii < 255; ii++)
	{
		if (_UsedValues[ii] == 0)
			return ii;
	}
	return -1;
}

MySensorsBase::_tMySensorNode* MySensorsBase::FindNode(const int nodeID)
{
	std::map<int, _tMySensorNode>::iterator itt;
	itt = m_nodes.find(nodeID);
	return (itt == m_nodes.end()) ? NULL : &itt->second;
}

MySensorsBase::_tMySensorNode* MySensorsBase::InsertNode(const int nodeID)
{
	_tMySensorNode mNode;
	mNode.nodeID = nodeID;
	mNode.SketchName = "Unknown";
	mNode.SketchVersion = "1.0";
	mNode.lastreceived = time(NULL);
	m_nodes[mNode.nodeID] = mNode;
	Add2Database(mNode.nodeID, mNode.SketchName, mNode.SketchVersion);
	return FindNode(nodeID);
}

//Find sensor with childID+devType
MySensorsBase::_tMySensorSensor* MySensorsBase::FindSensor(_tMySensorNode *pNode, const int childID, _eSetType devType)
{
	std::vector<_tMySensorSensor>::iterator itt;
	for (itt = pNode->m_sensors.begin(); itt != pNode->m_sensors.end(); ++itt)
	{
		if (
			(itt->childID == childID)&&
			(itt->devType == devType)
			)
			return &*itt;
	}
	return NULL;
}

//Find any sensor with devType
MySensorsBase::_tMySensorSensor* MySensorsBase::FindSensor(const int nodeID, _eSetType devType)
{
	std::map<int, _tMySensorNode>::iterator ittNode;
	ittNode = m_nodes.find(nodeID);
	if (ittNode == m_nodes.end())
		return NULL;
	_tMySensorNode *pNode = &ittNode->second;
	std::vector<_tMySensorSensor>::iterator itt;
	for (itt = pNode->m_sensors.begin(); itt != pNode->m_sensors.end(); ++itt)
	{
		if (itt->devType == devType)
			return &*itt;
	}
	return NULL;
}

void MySensorsBase::UpdateNodeBatteryLevel(const int nodeID, const int Level)
{
	std::map<int, _tMySensorNode>::iterator ittNode = m_nodes.find(nodeID);
	if (ittNode == m_nodes.end())
		return; //Not found
	_tMySensorNode *pNode = &ittNode->second;
	std::vector<_tMySensorSensor>::iterator itt;
	for (itt = pNode->m_sensors.begin(); itt != pNode->m_sensors.end(); ++itt)
	{
		itt->hasBattery = true;
		itt->batValue = Level;
	}
}

void MySensorsBase::MakeAndSendWindSensor(const int nodeID)
{
	bool bHaveTemp = false;
	int ChildID = 0;

	float fWind = 0;
	float fGust = 0;
	float fTemp = 0;
	float fChill = 0;
	int iDirection = 0;
	int iBatteryLevel = 255;

	_tMySensorSensor *pSensor;
	pSensor = FindSensor(nodeID, V_WIND);
	if (!pSensor)
		return;
	if (!pSensor->bValidValue)
		return;
	fWind = fGust = pSensor->floatValue;
	ChildID = pSensor->childID;
	iBatteryLevel = pSensor->batValue;

	pSensor = FindSensor(nodeID, V_DIRECTION);
	if (!pSensor)
		return;
	if (!pSensor->bValidValue)
		return;
	iDirection = pSensor->intvalue;

	pSensor = FindSensor(nodeID, V_GUST);
	if (pSensor)
	{
		if (!pSensor->bValidValue)
			return;
		fGust = pSensor->floatValue;
	}

	pSensor = FindSensor(nodeID, V_TEMP);
	if (pSensor)
	{
		if (!pSensor->bValidValue)
			return;
		bHaveTemp = true;
		fTemp = fChill = pSensor->floatValue;
		if ((fTemp < 10.0) && (fWind >= 1.4))
		{
			fChill = 13.12f + 0.6215f*fTemp - 11.37f*pow(fWind*3.6f, 0.16f) + 0.3965f*fTemp*pow(fWind*3.6f, 0.16f);
		}
	}
	int cNode = (nodeID << 8) | ChildID;
	SendWind(cNode, iBatteryLevel, float(iDirection), fWind, fGust, fTemp, fChill, bHaveTemp, "Wind");
}

void MySensorsBase::SendSensor2Domoticz(const _tMySensorNode *pNode, const _tMySensorSensor *pSensor)
{
	std::string devname;
	m_iLastSendNodeBatteryValue = 255;
	if (pSensor->hasBattery)
	{
		m_iLastSendNodeBatteryValue = pSensor->batValue;
	}
	int cNode = (pSensor->nodeID << 8) | pSensor->childID;

	switch (pSensor->devType)
	{
	case V_TEMP:
	{
		_tMySensorSensor *pSensorHum = FindSensor(pSensor->nodeID, V_HUM);
		_tMySensorSensor *pSensorBaro = FindSensor(pSensor->nodeID, V_PRESSURE);
		if (pSensorHum && pSensorBaro)
		{
			if (pSensorHum->bValidValue && pSensorBaro->bValidValue)
			{
				int forecast = bmpbaroforecast_unknown;
				_tMySensorSensor *pSensorForecast = FindSensor(pSensor->nodeID, V_FORECAST);
				if (pSensorForecast)
					forecast = pSensorForecast->intvalue;
				if (forecast == bmpbaroforecast_cloudy)
				{
					if (pSensorBaro->floatValue < 1010)
						forecast = bmpbaroforecast_rain;
				}

				//We are using the TempHumBaro Float type now, convert the forecast
				int nforecast = wsbaroforcast_some_clouds;
				float pressure = pSensorBaro->floatValue;
				if (pressure <= 980)
					nforecast = wsbaroforcast_heavy_rain;
				else if (pressure <= 995)
				{
					if (pSensor->floatValue > 1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				else if (pressure >= 1029)
					nforecast = wsbaroforcast_sunny;
				switch (forecast)
				{
				case bmpbaroforecast_sunny:
					nforecast = wsbaroforcast_sunny;
					break;
				case bmpbaroforecast_cloudy:
					nforecast = wsbaroforcast_cloudy;
					break;
				case bmpbaroforecast_thunderstorm:
					nforecast = wsbaroforcast_heavy_rain;
					break;
				case bmpbaroforecast_rain:
					if (pSensor->floatValue>1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				SendTempHumBaroSensorFloat(cNode, pSensor->batValue, pSensor->floatValue, pSensorHum->intvalue, pSensorBaro->floatValue, nforecast, "TempHumBaro");
			}
		}
		else if (pSensorHum) {
			if (pSensorHum->bValidValue)
			{
				SendTempHumSensor(cNode, pSensor->batValue, pSensor->floatValue, pSensorHum->intvalue, "TempHum");
			}
		}
		else
		{
			SendTempSensor(cNode, pSensor->batValue, pSensor->floatValue,"Temp");
		}
	}
	break;
	case V_HUM:
	{
		_tMySensorSensor *pSensorTemp = FindSensor(pSensor->nodeID, V_TEMP);
		_tMySensorSensor *pSensorBaro = FindSensor(pSensor->nodeID, V_PRESSURE);
		int forecast = bmpbaroforecast_unknown;
		_tMySensorSensor *pSensorForecast = FindSensor(pSensor->nodeID, V_FORECAST);
		if (pSensorForecast)
			forecast = pSensorForecast->intvalue;
		if (forecast == bmpbaroforecast_cloudy)
		{
			if (pSensorBaro->floatValue < 1010)
				forecast = bmpbaroforecast_rain;
		}
		if (pSensorTemp && pSensorBaro)
		{
			if (pSensorTemp->bValidValue && pSensorBaro->bValidValue)
			{
				cNode = (pSensorTemp->nodeID << 8) | pSensorTemp->childID;

				//We are using the TempHumBaro Float type now, convert the forecast
				int nforecast = wsbaroforcast_some_clouds;
				float pressure = pSensorBaro->floatValue;
				if (pressure <= 980)
					nforecast = wsbaroforcast_heavy_rain;
				else if (pressure <= 995)
				{
					if (pSensorTemp->floatValue > 1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				else if (pressure >= 1029)
					nforecast = wsbaroforcast_sunny;
				switch (forecast)
				{
				case bmpbaroforecast_sunny:
					nforecast = wsbaroforcast_sunny;
					break;
				case bmpbaroforecast_cloudy:
					nforecast = wsbaroforcast_cloudy;
					break;
				case bmpbaroforecast_thunderstorm:
					nforecast = wsbaroforcast_heavy_rain;
					break;
				case bmpbaroforecast_rain:
					if (pSensorTemp->floatValue > 1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				SendTempHumBaroSensorFloat(cNode, pSensorTemp->batValue, pSensorTemp->floatValue, pSensor->intvalue, pSensorBaro->floatValue, nforecast, "TempHumBaro");
			}
		}
		else if (pSensorTemp) {
			if (pSensorTemp->bValidValue)
			{
				cNode = (pSensorTemp->nodeID << 8) | pSensorTemp->childID;
				SendTempHumSensor(cNode, pSensorTemp->batValue, pSensorTemp->floatValue, pSensor->intvalue, "TempHum");
			}
		}
		else
		{
			SendHumiditySensor(cNode, pSensor->batValue, pSensor->intvalue);
		}
	}
	break;
	case V_PRESSURE:
	{
		_tMySensorSensor *pSensorTemp = FindSensor(pSensor->nodeID, V_TEMP);
		_tMySensorSensor *pSensorHum = FindSensor(pSensor->nodeID, V_HUM);
		int forecast = bmpbaroforecast_unknown;
		_tMySensorSensor *pSensorForecast = FindSensor(pSensor->nodeID, V_FORECAST);
		if (pSensorForecast)
			forecast = pSensorForecast->intvalue;
		if (forecast == bmpbaroforecast_cloudy)
		{
			if (pSensor->floatValue < 1010)
				forecast = bmpbaroforecast_rain;
		}
		if (pSensorTemp && pSensorHum)
		{
			if (pSensorTemp->bValidValue && pSensorHum->bValidValue)
			{
				cNode = (pSensorTemp->nodeID << 8) | pSensorTemp->childID;
				//We are using the TempHumBaro Float type now, convert the forecast
				int nforecast = wsbaroforcast_some_clouds;
				float pressure = pSensor->floatValue;
				if (pressure <= 980)
					nforecast = wsbaroforcast_heavy_rain;
				else if (pressure <= 995)
				{
					if (pSensorTemp->floatValue > 1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				else if (pressure >= 1029)
					nforecast = wsbaroforcast_sunny;
				switch (forecast)
				{
				case bmpbaroforecast_sunny:
					nforecast = wsbaroforcast_sunny;
					break;
				case bmpbaroforecast_cloudy:
					nforecast = wsbaroforcast_cloudy;
					break;
				case bmpbaroforecast_thunderstorm:
					nforecast = wsbaroforcast_heavy_rain;
					break;
				case bmpbaroforecast_rain:
					if (pSensorTemp->floatValue > 1)
						nforecast = wsbaroforcast_rain;
					else
						nforecast = wsbaroforcast_snow;
					break;
				}
				SendTempHumBaroSensorFloat(cNode, pSensorTemp->batValue, pSensorTemp->floatValue, pSensorHum->intvalue, pSensor->floatValue, nforecast, "TempHumBaro");
			}
		}
		else
			SendBaroSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->floatValue, forecast);
	}
	break;
	case V_TRIPPED:
		//	Tripped status of a security sensor. 1 = Tripped, 0 = Untripped
		UpdateSwitch(pSensor->nodeID, pSensor->childID, (pSensor->intvalue == 1), 100, "Security Sensor");
		break;
	case V_ARMED:
		//Armed status of a security sensor. 1 = Armed, 0 = Bypassed
		UpdateSwitch(pSensor->nodeID, pSensor->childID, (pSensor->intvalue == 1), 100, "Security Sensor");
		break;
	case V_LOCK_STATUS:
		//Lock status. 1 = Locked, 0 = Unlocked
		UpdateSwitch(pSensor->nodeID, pSensor->childID, (pSensor->intvalue == 1), 100, "Lock Sensor");
		break;
	case V_LIGHT:
		//	Light status. 0 = off 1 = on
		UpdateSwitch(pSensor->nodeID, pSensor->childID, (pSensor->intvalue != 0), 100, "Light");
		break;
	case V_SCENE_ON:
		UpdateSwitch(pSensor->nodeID, pSensor->childID + pSensor->intvalue, true, 100, "Scene");
		break;
	case V_SCENE_OFF:
		UpdateSwitch(pSensor->nodeID, pSensor->childID + pSensor->intvalue, false, 100, "Scene");
		break;
	case V_DIMMER:
		//	Dimmer value. 0 - 100 %
		{
			int level = pSensor->intvalue;
			UpdateSwitch(pSensor->nodeID, pSensor->childID, (level != 0), level, "Light");
		}
		break;
	case V_RGB:
		//RRGGBB
		SendRGBWSwitch(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->intvalue, false, "RGB Light");
		break;
	case V_RGBW:
		//RRGGBBWW
		SendRGBWSwitch(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->intvalue, true, "RGBW Light");
		break;
	case V_UP:
	case V_DOWN:
	case V_STOP:
		SendBlindSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->intvalue, "Blinds/Window");
		break;
	case V_LIGHT_LEVEL:
		{
			_tLightMeter lmeter;
			lmeter.id1 = 0;
			lmeter.id2 = 0;
			lmeter.id3 = 0;
			lmeter.id4 = pSensor->nodeID;
			lmeter.dunit = pSensor->childID;
			lmeter.fLux = pSensor->floatValue;
			lmeter.battery_level = pSensor->batValue;
			if (pSensor->hasBattery)
				lmeter.battery_level = pSensor->batValue;
			sDecodeRXMessage(this, (const unsigned char *)&lmeter);
		}
		break;
	case V_DUST_LEVEL:
		{
			_tAirQualityMeter meter;
			meter.len = sizeof(_tAirQualityMeter) - 1;
			meter.type = pTypeAirQuality;
			meter.subtype = sTypeVoltcraft;
			meter.airquality = pSensor->intvalue;
			meter.id1 = pSensor->nodeID;
			meter.id2 = pSensor->childID;
			sDecodeRXMessage(this, (const unsigned char *)&meter);
		}
		break;
	case V_RAIN:
		SendRainSensor(cNode, pSensor->batValue, pSensor->intvalue,"Rain");
		break;
	case V_WATT:
		{
			_tMySensorSensor *pSensorKwh = FindSensor(pSensor->nodeID, V_KWH);
			if (pSensorKwh) {
				SendKwhMeter(pSensorKwh->nodeID, pSensorKwh->childID, pSensorKwh->batValue, pSensor->floatValue / 1000.0f, pSensorKwh->floatValue, "Meter");
			}
			else {
				_tUsageMeter umeter;
				umeter.id1 = 0;
				umeter.id2 = 0;
				umeter.id3 = 0;
				umeter.id4 = pSensor->nodeID;
				umeter.dunit = pSensor->childID;
				umeter.fusage = pSensor->floatValue/1000.0f;
				sDecodeRXMessage(this, (const unsigned char *)&umeter);
			}
		}
		break;
	case V_KWH:
		{
			_tMySensorSensor *pSensorWatt = FindSensor(pSensor->nodeID, V_WATT);
			if (pSensorWatt) {
				SendKwhMeter(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensorWatt->floatValue / 1000.0f, pSensor->floatValue, "Meter");
			}
			else {
				SendKwhMeter(pSensor->nodeID, pSensor->childID, pSensor->batValue, 0, pSensor->floatValue, "Meter");
			}
		}
		break;
	case V_DISTANCE:
		SendDistanceSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->floatValue);
		break;
	case V_FLOW:
		//Flow of water in meter
		while (1==0);
		break;
	case V_VOLUME:
		//Water Volume
		SendMeterSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->floatValue);
		break;
	case V_VOLTAGE:
		devname = "Voltage";
		SendVoltageSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->floatValue, devname);
		break;
	case V_UV:
		SendUVSensor(pSensor->nodeID, pSensor->childID, pSensor->batValue, pSensor->floatValue);
		break;
	case V_CURRENT:
		devname = "Current";
		SendCurrentSensor(cNode, pSensor->batValue, pSensor->floatValue, 0, 0, devname);
		break;
	case V_FORECAST:
		{
			_tMySensorSensor *pSensorBaro = FindSensor(pSensor->nodeID, V_PRESSURE);
			if (pSensorBaro)
			{
				if (pSensorBaro->bValidValue)
				{
					int forecast = pSensor->intvalue;
					if (forecast == bmpbaroforecast_cloudy)
					{
						if (pSensor->floatValue < 1010)
							forecast = bmpbaroforecast_rain;
					}
					SendBaroSensor(pSensorBaro->nodeID, pSensorBaro->childID, pSensorBaro->batValue, pSensorBaro->floatValue, forecast);
				}
			}
			else
			{
				std::stringstream sstr;
				sstr << pSensor->nodeID;
				m_sql.UpdateValue(m_HwdID, sstr.str().c_str(), pSensor->childID, pTypeGeneral, sTypeTextStatus, 12, pSensor->batValue, 0, pSensor->stringValue.c_str(), devname);
			}
		}
		break;
	case V_WIND:
	case V_GUST:
	case V_DIRECTION:
		MakeAndSendWindSensor(pSensor->nodeID);
		break;
	}
}

void MySensorsBase::ParseData(const unsigned char *pData, int Len)
{
	int ii=0;
	while (ii<Len)
	{
		const unsigned char c = pData[ii];
		if(c == 0x0d)
		{
			ii++;
			continue;
		}

		if(c == 0x0a || m_bufferpos == sizeof(m_buffer) - 1)
		{
			// discard newline, close string, parse line and clear it.
			if(m_bufferpos > 0) m_buffer[m_bufferpos] = 0;
			ParseLine();
			m_bufferpos = 0;
		}
		else
		{
			m_buffer[m_bufferpos] = c;
			m_bufferpos++;
		}
		ii++;
	}
}

void MySensorsBase::UpdateSwitch(const unsigned char Idx, const int SubUnit, const bool bOn, const double Level, const std::string &defaultname)
{
	bool bDeviceExits = true;
	double rlevel = (15.0 / 100)*Level;
	int level = int(rlevel);

	char szIdx[10];
	sprintf(szIdx, "%X%02X%02X%02X", 0, 0, 0, Idx);
	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT Name,nValue,sValue FROM DeviceStatus WHERE (HardwareID==%d) AND (DeviceID=='%q') AND (Unit==%d) AND (Type==%d) AND (Subtype==%d)", m_HwdID, szIdx, SubUnit, int(pTypeLighting2), int(sTypeAC));
	if (result.size() < 1)
	{
		bDeviceExits = false;
	}
	else
	{
		//check if we have a change, if not do not update it
		int nvalue = atoi(result[0][1].c_str());
		if ((!bOn) && (nvalue == 0))
			return;
		if ((bOn && (nvalue != 0)))
		{
			//Check Level
			int slevel = atoi(result[0][2].c_str());
			if (slevel==level)
				return;
		}
	}

	//Send as Lighting 2
	tRBUF lcmd;
	memset(&lcmd, 0, sizeof(RBUF));
	lcmd.LIGHTING2.packetlength = sizeof(lcmd.LIGHTING2) - 1;
	lcmd.LIGHTING2.packettype = pTypeLighting2;
	lcmd.LIGHTING2.subtype = sTypeAC;
	lcmd.LIGHTING2.id1 = 0;
	lcmd.LIGHTING2.id2 = 0;
	lcmd.LIGHTING2.id3 = 0;
	lcmd.LIGHTING2.id4 = Idx;
	lcmd.LIGHTING2.unitcode = SubUnit;
	if (!bOn)
	{
		lcmd.LIGHTING2.cmnd = light2_sOff;
	}
	else
	{
		lcmd.LIGHTING2.cmnd = light2_sOn;
	}
	lcmd.LIGHTING2.level = level;
	lcmd.LIGHTING2.filler = 0;
	lcmd.LIGHTING2.rssi = 12;
	sDecodeRXMessage(this, (const unsigned char *)&lcmd.LIGHTING2);

	if (!bDeviceExits)
	{
		//Assign default name for device
		m_sql.safe_query("UPDATE DeviceStatus SET Name='%q' WHERE (HardwareID==%d) AND (DeviceID=='%q') AND (Unit==%d) AND (Type==%d) AND (Subtype==%d)", defaultname.c_str(), m_HwdID, szIdx, SubUnit, int(pTypeLighting2), int(sTypeAC));
	}
}

bool MySensorsBase::GetBlindsValue(const int NodeID, const int ChildID, int &blind_value)
{
	char szIdx[10];
	sprintf(szIdx, "%02X%02X%02X", 0, 0, NodeID);
	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT nValue FROM DeviceStatus WHERE (HardwareID==%d) AND (DeviceID=='%q') AND (Unit==%d)", m_HwdID, szIdx, ChildID);
	if (result.size() < 1)
		return false;
	blind_value = atoi(result[0][0].c_str());
	return true;
}

bool MySensorsBase::GetSwitchValue(const unsigned char Idx, const int SubUnit, const int sub_type, std::string &sSwitchValue)
{
	char szIdx[10];
	if ((sub_type == V_RGB) || (sub_type == V_RGBW))
	{
		if (Idx==1)
			sprintf(szIdx, "%d", 1);
		else
			sprintf(szIdx, "%08x", Idx);
	}
	else
	{
		sprintf(szIdx, "%X%02X%02X%02X", 0, 0, 0, Idx);
	}
	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT Name,nValue,sValue FROM DeviceStatus WHERE (HardwareID==%d) AND (DeviceID=='%q') AND (Unit==%d)", m_HwdID, szIdx, SubUnit);
	if (result.size() < 1)
		return false;
	int nvalue = atoi(result[0][1].c_str());
	if ((sub_type == V_LIGHT) || (sub_type == V_TRIPPED))
	{
		sSwitchValue = (nvalue == light2_sOn) ? "1" : "0";
		return true;
	}
	else if ((sub_type == V_RGB) || (sub_type == V_RGBW))
	{
		sSwitchValue = (nvalue == Limitless_LedOn) ? "1" : "0";
		return true;
	}

	int slevel = atoi(result[0][2].c_str());
	std::stringstream sstr;
	sstr << int(slevel * 100 / 15);
	sSwitchValue = sstr.str();
	return true;
}

void MySensorsBase::SendCommand(const int NodeID, const int ChildID, const _eMessageType messageType, const int SubType, const std::string &Payload)
{
	std::stringstream sstr;
	sstr << NodeID << ";" << ChildID << ";" << int(messageType) << ";0;" << SubType << ";" << Payload << '\n';
	WriteInt(sstr.str());
}

bool MySensorsBase::WriteToHardware(const char *pdata, const unsigned char length)
{
	tRBUF *pCmd = (tRBUF *)pdata;
	if (pCmd->LIGHTING2.packettype == pTypeLighting2)
	{
		//Light command

		int node_id = pCmd->LIGHTING2.id4;
		int child_sensor_id = pCmd->LIGHTING2.unitcode;

		if (_tMySensorNode *pNode = FindNode(node_id))
		{
			int light_command = pCmd->LIGHTING2.cmnd;
			if ((pCmd->LIGHTING2.cmnd == light2_sSetLevel) && (pCmd->LIGHTING2.level == 0))
			{
				light_command = light2_sOff;
			}
			else if ((pCmd->LIGHTING2.cmnd == light2_sSetLevel) && (pCmd->LIGHTING2.level == 255))
			{
				light_command = light2_sOn;
			}

			if ((light_command == light2_sOn) || (light_command == light2_sOff))
			{
				if (pNode->FindType(S_LOCK))
				{
					//Door lock
					std::string lState = (light_command == light2_sOn) ? "0" : "1";
					SendCommand(node_id, child_sensor_id, MT_Set, V_LOCK_STATUS, lState);
				}
				else
				{
					//normal
					std::string lState = (light_command == light2_sOn) ? "1" : "0";
					SendCommand(node_id, child_sensor_id, MT_Set, V_LIGHT, lState);
				}
			}
			else if (light_command == light2_sSetLevel)
			{
				float fvalue = (100.0f / 15.0f)*float(pCmd->LIGHTING2.level);
				if (fvalue > 100.0f)
					fvalue = 100.0f; //99 is fully on
				int svalue = round(fvalue);

				std::stringstream sstr;
				sstr << svalue;
				SendCommand(node_id, child_sensor_id, MT_Set, V_DIMMER, sstr.str());
			}
		}
		else {
			_log.Log(LOG_ERROR, "MySensors: Light command received for unknown node_id: %d", node_id);
			return false;
		}
	}
	else if (pCmd->LIGHTING2.packettype == pTypeLimitlessLights)
	{
		//RGW/RGBW command
		_tLimitlessLights *pLed = (_tLimitlessLights *)pdata;
		unsigned char ID1 = (unsigned char)((pLed->id & 0xFF000000) >> 24);
		unsigned char ID2 = (unsigned char)((pLed->id & 0x00FF0000) >> 16);
		unsigned char ID3 = (unsigned char)((pLed->id & 0x0000FF00) >> 8);
		unsigned char ID4 = (unsigned char)pLed->id & 0x000000FF;

		int node_id = (ID3 << 8) | ID4;
		int child_sensor_id = pLed->dunit;

		if (_tMySensorNode *pNode = FindNode(node_id))
		{
			bool bIsRGBW = (FindSensor(pNode, child_sensor_id, V_RGBW) != NULL);
			if (pLed->command == Limitless_SetRGBColour)
			{
				int red, green, blue;

				float cHue = (360.0f / 255.0f)*float(pLed->value);//hue given was in range of 0-255
				int Brightness = 100;
				int dMax = round((255.0f / 100.0f)*float(Brightness));
				hue2rgb(cHue, red, green, blue, dMax);
				std::stringstream sstr;
				sstr << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << red
					<< std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << green
					<< std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << blue;
				SendCommand(node_id, child_sensor_id, MT_Set, (bIsRGBW == true) ? V_RGBW : V_RGB, sstr.str());
			}
			else if (pLed->command == Limitless_SetColorToWhite)
			{
				std::stringstream sstr;
				int Brightness = 100;
				int wWhite = round((255.0f / 100.0f)*float(Brightness));
				if (!bIsRGBW)
				{
					sstr << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << wWhite
						<< std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << wWhite
						<< std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << wWhite;
				}
				else
				{
					sstr << "#000000"
						<< std::setw(2) << std::uppercase << std::hex << std::setfill('0') << std::hex << wWhite;
				}
				SendCommand(node_id, child_sensor_id, MT_Set, (bIsRGBW == true) ? V_RGBW : V_RGB, sstr.str());
			}
			else if (pLed->command == Limitless_SetBrightnessLevel)
			{
				float fvalue = pLed->value;
				int svalue = round(fvalue);
				if (svalue > 100)
					svalue = 100;
				std::stringstream sstr;
				sstr << svalue;
				SendCommand(node_id, child_sensor_id, MT_Set, V_DIMMER, sstr.str());
			}
			else if ((pLed->command == Limitless_LedOff) || (pLed->command == Limitless_LedOn))
			{
				std::string lState = (pLed->command == Limitless_LedOn) ? "1" : "0";
				SendCommand(node_id, child_sensor_id, MT_Set, V_LIGHT, lState);
			}
		}
		else
		{
			_log.Log(LOG_ERROR, "MySensors: Light command received for unknown node_id: %d", node_id);
			return false;
		}
	}
	else if (pCmd->BLINDS1.packettype == pTypeBlinds)
	{
		//Blinds/Window command
		int node_id = pCmd->BLINDS1.id3;
		int child_sensor_id = pCmd->BLINDS1.unitcode;

		if (_tMySensorNode *pNode = FindNode(node_id))
		{
			if (pCmd->BLINDS1.cmnd == blinds_sOpen)
			{
				SendCommand(node_id, child_sensor_id, MT_Set, V_UP, "");
			}
			else if (pCmd->BLINDS1.cmnd == blinds_sClose)
			{
				SendCommand(node_id, child_sensor_id, MT_Set, V_DOWN, "");
			}
			else if (pCmd->BLINDS1.cmnd == blinds_sStop)
			{
				SendCommand(node_id, child_sensor_id, MT_Set, V_STOP, "");
			}
		}
		else {
			_log.Log(LOG_ERROR, "MySensors: Blinds/Window command received for unknown node_id: %d", node_id);
			return false;
		}
	}
	return true;
}

void MySensorsBase::UpdateVar(const int NodeID, const int ChildID, const int VarID, const std::string &svalue)
{
	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT ROWID FROM MySensorsVars WHERE (HardwareID=%d) AND (NodeID=%d) AND (ChildID=%d) AND (VarID=%d)", m_HwdID, NodeID, ChildID, VarID);
	if (result.size() == 0)
	{
		//Insert
		m_sql.safe_query("INSERT INTO MySensorsVars (HardwareID, NodeID, ChildID, VarID, [Value]) VALUES (%d, %d, %d, %d,'%q')", m_HwdID, NodeID, ChildID, VarID, svalue.c_str());
	}
	else
	{
		//Update
		m_sql.safe_query("UPDATE MySensorsVars SET [Value]='%q' WHERE (ROWID = '%q')", svalue.c_str(), result[0][0].c_str());
	}

}

bool MySensorsBase::GetVar(const int NodeID, const int ChildID, const int VarID, std::string &sValue)
{
	std::vector<std::vector<std::string> > result;
	result = m_sql.safe_query("SELECT [Value] FROM MySensorsVars WHERE (HardwareID=%d) AND (NodeID=%d) AND (ChildID=%d) AND (VarID=%d)", m_HwdID, NodeID, ChildID, VarID);
	if (result.size() < 1)
		return false;
	std::vector<std::string> sd = result[0];
	sValue = sd[0];
	return true;
}

void MySensorsBase::ParseLine()
{
	if (m_bufferpos<2)
		return;
	std::string sLine((char*)&m_buffer);

	//_log.Log(LOG_STATUS, sLine.c_str());

	std::vector<std::string> results;
	StringSplit(sLine, ";", results);
	if (results.size()<5)
		return; //invalid data

	int node_id = atoi(results[0].c_str());
	int child_sensor_id = atoi(results[1].c_str());
	_eMessageType message_type = (_eMessageType)atoi(results[2].c_str());
	int ack = atoi(results[3].c_str());
	int sub_type = atoi(results[4].c_str());
	std::string payload = "";
	if (results.size()>=6)
		payload=results[5];

	std::stringstream sstr;
#ifdef _DEBUG
	_log.Log(LOG_NORM, "MySensors: NodeID: %d, ChildID: %d, MessageType: %d, Ack: %d, SubType: %d, Payload: %s",node_id,child_sensor_id,message_type,ack,sub_type,payload.c_str());
#endif

	if (message_type == MT_Internal)
	{
		switch (sub_type)
		{
		case I_ID_REQUEST:
			{
				//Set a unique node id from the controller
				int newID = FindNextNodeID();
				if (newID != -1)
				{
					sstr << newID;
					SendCommand(node_id, child_sensor_id, message_type, I_ID_RESPONSE, sstr.str());
				}
			}
			break;
		case I_CONFIG:
			// (M)etric or (I)mperal back to sensor.
			//Set a unique node id from the controller
			SendCommand(node_id, child_sensor_id, message_type, I_CONFIG, "M");
			break;
		case I_SKETCH_NAME:
			_log.Log(LOG_STATUS, "MySensors: Node: %d, Sketch Name: %s", node_id, payload.c_str());
			if (_tMySensorNode *pNode = FindNode(node_id))
			{
				DatabaseUpdateSketchName(node_id, payload);
			}
			else
			{
				//Unknown Node
				InsertNode(node_id);
				DatabaseUpdateSketchName(node_id, payload);
			}
			break;
		case I_SKETCH_VERSION:
			_log.Log(LOG_STATUS, "MySensors: Node: %d, Sketch Version: %s", node_id, payload.c_str());
			if (_tMySensorNode *pNode = FindNode(node_id))
			{
				DatabaseUpdateSketchVersion(node_id, payload);
			}
			else
			{
				//Unknown Node
				InsertNode(node_id);
				DatabaseUpdateSketchVersion(node_id, payload);
			}
			break;
		case I_BATTERY_LEVEL:
			UpdateNodeBatteryLevel(node_id, atoi(payload.c_str()));
			break;
		case I_LOG_MESSAGE:
			break;
		case I_GATEWAY_READY:
			_log.Log(LOG_NORM, "MySensors: Gateway Ready...");
			break;
		case I_TIME:
			//send time in seconds from 1970 with timezone offset
			{
				time_t atime = mytime(NULL);
				struct tm ltime;
				localtime_r(&atime, &ltime);
				sstr << mktime(&ltime);
				SendCommand(node_id, child_sensor_id, message_type, I_TIME, sstr.str());
			}
			break;
		default:
			while (1==0);
			break;
		}
	}
	else if (message_type == MT_Set)
	{
		_tMySensorNode *pNode = FindNode(node_id);
		if (pNode == NULL)
		{
			pNode = InsertNode(node_id);
			if (pNode == NULL)
				return;
		}
		pNode->lastreceived = mytime(NULL);

		_tMySensorSensor *pSensor = FindSensor(pNode, child_sensor_id, (_eSetType)sub_type);
		if (pSensor == NULL)
		{
			//Unknown sensor, add it to the system
			_tMySensorSensor mSensor;
			mSensor.nodeID = node_id;
			mSensor.childID = child_sensor_id;
			mSensor.devType = (_eSetType)sub_type;
			pNode->m_sensors.push_back(mSensor);
			pSensor = FindSensor(pNode, child_sensor_id, mSensor.devType);
			if (pSensor == NULL)
				return;
		}
		pSensor->lastreceived = mytime(NULL);
		pSensor->devType = (_eSetType)sub_type;
		pSensor->bValidValue = true;
		bool bHaveValue = false;
		switch (sub_type)
		{
		case V_TEMP:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_HUM:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_PRESSURE:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_VAR1: //Custom value
		case V_VAR2:
		case V_VAR3:
		case V_VAR4:
		case V_VAR5:
			UpdateVar(node_id, child_sensor_id, sub_type, payload);
			break;
		case V_TRIPPED:
			//	Tripped status of a security sensor. 1 = Tripped, 0 = Untripped
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_ARMED:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_LOCK_STATUS:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_LIGHT:
			//	Light status. 0 = off 1 = on
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_SCENE_ON:
			//	Scene On
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_SCENE_OFF:
			//	Scene Off
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_RGB:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_RGBW:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_DIMMER:
			//	Dimmer value. 0 - 100 %
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_UP:
			pSensor->intvalue = blinds_sOpen;
			bHaveValue = true;
			break;
		case V_DOWN:
			pSensor->intvalue = blinds_sClose;
			bHaveValue = true;
			break;
		case V_STOP:
			pSensor->intvalue = blinds_sStop;
			bHaveValue = true;
			break;
		case V_DUST_LEVEL:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_RAIN:
			pSensor->intvalue = atoi(payload.c_str());
			bHaveValue = true;
			break;
		case V_WATT:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_KWH:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_DISTANCE:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_FLOW:
			//Flow of water in meter
			while (1==0);
			break;
		case V_VOLUME:
			//Water Volume
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_WIND:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_GUST:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_DIRECTION:
			pSensor->intvalue = round(atof(payload.c_str()));
			bHaveValue = true;
			break;
		case V_LIGHT_LEVEL:
			pSensor->floatValue = (float)atof(payload.c_str());
			/*
			//convert percentage to 1000 scale
			pSensor->floatValue = (1000.0f / 100.0f)*pSensor->floatValue;
			if (pSensor->floatValue > 1000.0f)
				pSensor->floatValue = 1000.0f;
			*/
			bHaveValue = true;
			break;
		case V_FORECAST:
			pSensor->stringValue = payload;
			//	Whether forecast.One of "stable", "sunny", "cloudy", "unstable", "thunderstorm" or "unknown"
			pSensor->intvalue = bmpbaroforecast_unknown;
			if (pSensor->stringValue == "stable")
				pSensor->intvalue = bmpbaroforecast_stable;
			else if (pSensor->stringValue == "sunny")
				pSensor->intvalue = bmpbaroforecast_sunny;
			else if (pSensor->stringValue == "cloudy")
				pSensor->intvalue = bmpbaroforecast_cloudy;
			else if (pSensor->stringValue == "unstable")
				pSensor->intvalue = bmpbaroforecast_unstable;
			else if (pSensor->stringValue == "thunderstorm")
				pSensor->intvalue = bmpbaroforecast_thunderstorm;
			else if (pSensor->stringValue == "unknown")
				pSensor->intvalue = bmpbaroforecast_unknown;
			bHaveValue = true;
			break;
		case V_VOLTAGE:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_UV:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		case V_CURRENT:
			pSensor->floatValue = (float)atof(payload.c_str());
			bHaveValue = true;
			break;
		default:
			if (sub_type > V_CURRENT)
			{
				_log.Log(LOG_ERROR, "MySensors: Unknown/Invalid sensor type (%d)",sub_type);
			}
			else
			{
				_log.Log(LOG_ERROR, "MySensors: Unhandled sensor (sub-type=%d), please report with log!",sub_type);
			}
			break;
		}
		if (bHaveValue)
		{
			SendSensor2Domoticz(pNode, pSensor);
		}
	}
	else if (message_type == MT_Presentation)
	{
		//Ignored for now
		if ((node_id == 255) || (child_sensor_id == 255))
			return;

		bool bDoAdd = false;

		_ePresentationType pType = (_ePresentationType)sub_type;

		switch (sub_type)
		{
		case S_TEMP:
			sub_type = V_TEMP;
			bDoAdd = true;
			break;
		case S_HUM:
			sub_type = V_HUM;
			bDoAdd = true;
			break;
		case S_BARO:
			sub_type = V_PRESSURE;
			bDoAdd = true;
			break;
		case S_LOCK:
		case S_LIGHT:
		case S_DIMMER:
		case S_SMOKE:
		case S_DOOR:
		case S_SCENE_CONTROLLER:
			sub_type = V_SCENE_ON;
			bDoAdd = true;
			break;
		case S_MOTION:
			sub_type = V_TRIPPED;
			bDoAdd = true;
			break;
		case S_COVER:
			sub_type = V_UP;
			bDoAdd = true;
			break;
		case S_RGB_LIGHT:
		case S_COLOR_SENSOR:
			sub_type = V_RGB;
			bDoAdd = true;
			break;
		case S_RGBW_LIGHT:
			sub_type = V_RGBW;
			bDoAdd = true;
			break;
		}
		if (!bDoAdd)
			return;

		_tMySensorNode *pNode = FindNode(node_id);
		if (pNode == NULL)
		{
			pNode = InsertNode(node_id);
			if (pNode == NULL)
				return;
		}
		pNode->lastreceived = mytime(NULL);
		pNode->AddType(pType);

		_tMySensorSensor *pSensor = FindSensor(pNode, child_sensor_id, (_eSetType)sub_type);
		if (pSensor == NULL)
		{
			//Unknown sensor, add it to the system
			_tMySensorSensor mSensor;
			mSensor.nodeID = node_id;
			mSensor.childID = child_sensor_id;
			mSensor.devType = (_eSetType)sub_type;
			pNode->m_sensors.push_back(mSensor);
			pSensor = FindSensor(pNode, child_sensor_id, mSensor.devType);
			if (pSensor == NULL)
				return;
		}
		pSensor->lastreceived = mytime(NULL);
		pSensor->devType = (_eSetType)sub_type;
		pSensor->bValidValue = false;

		if ((sub_type == V_LIGHT) || (sub_type == V_RGB) || (sub_type == V_RGBW) || (sub_type == V_TRIPPED))
		{
			//Check if switch is already in the system, if not add it
			std::string sSwitchValue;
			if (!GetSwitchValue(node_id, child_sensor_id, sub_type, sSwitchValue))
			{
				//Add it to the system
				if (sub_type == V_LIGHT)
					UpdateSwitch(node_id, child_sensor_id, false, 100, "Light");
				else if (sub_type == V_TRIPPED)
					UpdateSwitch(node_id, child_sensor_id, false, 100, "Security Sensor");
				else
					SendRGBWSwitch(node_id, child_sensor_id, 255, 0, (sub_type == V_RGBW), (sub_type == V_RGBW) ? "RGBW Light" : "RGB Light");
			}
		}
		else if (sub_type == V_UP)
		{
			int blind_value;
			if (!GetBlindsValue(node_id, child_sensor_id, blind_value))
			{
				SendBlindSensor(node_id, child_sensor_id, 255, blinds_sOpen, "Blinds/Window");
			}
		}
	}
	else if (message_type == MT_Req)
	{
		//Request a variable
		std::string tmpstr;
		switch (sub_type)
		{
		case V_LIGHT:
		case V_DIMMER:
		case V_RGB:
		case V_RGBW:
			if (GetSwitchValue(node_id, child_sensor_id, sub_type, tmpstr))
				SendCommand(node_id, child_sensor_id, message_type, sub_type, tmpstr);
			break;
		case V_VAR1:
		case V_VAR2:
		case V_VAR3:
		case V_VAR4:
		case V_VAR5:
			//send back a previous stored custom variable
			tmpstr = "";
			GetVar(node_id, child_sensor_id, sub_type, tmpstr);
			SendCommand(node_id, child_sensor_id, message_type, sub_type, tmpstr);
			break;
		default:
			while (1==0);
			break;
		}
		while (1==0);
	}
	else {
		//Unhandled message type
		while (1==0);
	}
}

