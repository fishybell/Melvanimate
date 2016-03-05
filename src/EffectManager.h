#pragma once

#include <functional>
#include <ArduinoJson.h>
#include "EffectHandler.h"
#include "helperfunc.h"
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "mybus.h"


extern MyPixelBus * strip;
extern NeoPixelAnimator * animator;

#define PRESETS_FILE "/presets_"

//#define DebugEffectManager

#ifdef DebugEffectManager
#define DebugEffectManagerf(...) Serial.printf(__VA_ARGS__)
#else
#define DebugEffectManagerf(...) {}
#endif

using namespace helperfunc;
/* ------------------------------------------------------------------------
	Effect Mangager
	This is the base class for managing effects
--------------------------------------------------------------------------*/
class EffectHandler; 

class EffectManager
{

public:
	EffectManager();
	~EffectManager() {};

	bool Add(const char * name, EffectHandler* Handler, bool defaulteffect = false);

	inline bool Start()  { return Start(_toggleHandle); }
	bool Start(EffectHandler* handle);
	inline bool Start(const char * name)  { return Start(_findhandle(name)); }
	inline bool Start(const String name)  { return Start(name.c_str()); };
	
	void Refresh() ;
	bool Next() ;
	bool Previous(); 
	bool Stop() ;
	bool Pause() ;
	virtual void loop(); //  this can be overridden....  but should contain _process() function to operate... 

	void setWaitFn(std::function<bool()> Fn ) { _waitFn = Fn; } //  allow effects to wait until triggering next...

	EffectHandler* Current();  // returns current.. or the next in line.

	const uint16_t total() const { return _count;}
	const char* getName(uint8_t i);

	// preset hanlding
	bool Save(uint8_t ID, const char * name, bool overwrite = false);
	
	//bool Load(String value);  //  loads effects using file number... 1.2 3.4 etc.... needed for presets page, or presets that change running effect
	bool Load(uint8_t ID);    //  loads effect for the current running effect.. works from homepage... 
	bool Load(uint8_t File, uint8_t ID); //  they all call this eventually.... 
	bool removePreset(uint8_t ID);

	// fetches info from SPIFFS valid presests for current effect
	void removeAllpresets();
	void addAllpresets(JsonObject & root); 
	bool addCurrentPresets(JsonObject & root); 



	// NEW....manage all presets... 
	bool fillPresetArray();
	void dumpPresetArray(); 
	uint8_t nextFreePresetID(); 

	// maybe move this into a helper header file....



protected:

	EffectHandler*  _currentHandle;
	EffectHandler*  _firstHandle;
	EffectHandler*  _lastHandle;
	EffectHandler*  _NextInLine;
	EffectHandler*  _toggleHandle;
	EffectHandler*  _defaulteffecthandle; 

	uint16_t _count;

	void _process();  //  this must be called by any derived classes in loop. 

private:
	std::function<bool()>  _waitFn = nullptr;

	EffectHandler* _findhandle(const char * handle);
	
	struct Presets_s {
		~Presets_s(){
			if (name) {
				free((void*)name); 
			}
		}
		uint8_t file{0};
		uint8_t id{};
		EffectHandler * handle;
		const char * name; 
		void setname(const char * namein) {
			name = strdup(namein); 
		}
	};

	Presets_s * _presetS{nullptr}; 
	uint8_t _presetcountS{0}; 

	int _nextFreeFile(); 


};





/*

class MarqueeEffect : public SwitchEffect, public Color_property, public Brightness_property, public Palette_property, public Speed_property
{

public:
	MarqueeEffect(EffectHandlerFunction Fn) : SwitchEffect(Fn), _rotation(0), Color_property(this), Brightness_property(this), Palette_property(this), Speed_property(this)
	{
		_marqueeText = strdup("MELVANIMATE");
	};
	~MarqueeEffect()
	{
		free(_marqueeText);
		Serial.println("[~MarqueeEffect] marquee txt freed");
	}

	//  These functions just need to add and retrieve preset values from the json.
	bool load(JsonObject& root, const char *& ID) override ;

	bool addEffectJson(JsonObject& settings) override;
	bool parseJsonEffect(JsonObject& root) override;

	//  Specific Variables
//	void setBrightness(uint8_t bri)   {   _brightness = bri;  }
//	uint8_t getBrightness()  {    return _brightness;  }

//	void setColor(RgbColor color)   { _color = color;  }
//	RgbColor getColor()  {  return  _color;}

//	void setSpeed(uint8_t speed)   {   _speed = speed; }
//	uint8_t getSpeed()  {   return _speed;  }

	void setRotation(uint8_t rotation)   {   _rotation = rotation;}
	uint8_t getRotation()  {   return _rotation; }

	void setText(const char * text)   {   free(_marqueeText); _marqueeText = strdup(text);  }
	const char * getText()  { return _marqueeText; }

	//void setPalette(palette_type palette)   {   _palette = palette;  }
	Palette * getPalette()  {   return &_palette;  }
	//uint32_t _speed;

private:
	char * _marqueeText;
	//uint8_t _brightness;
	//palette_type _palette;
	uint8_t _rotation;
//	RgbColor _color;
	//Palette _palette;
};





class DummyEffect : public SwitchEffect
{

public:
	DummyEffect(EffectHandlerFunction Fn);

	//  These functions just need to add and retrieve preset values from the json.
	bool load(JsonObject& root, const char *& ID) override;
	bool addEffectJson(JsonObject& settings) override;
	bool parseJsonEffect(JsonObject& root) override;

private:
	Palette _palette;

};


class CascadeEffect : public SwitchEffect, public Color_property, public Brightness_property, public Palette_property
{

public:
	CascadeEffect(EffectHandlerFunction Fn): SwitchEffect(Fn), Color_property(this), Brightness_property(this),
		Palette_property(this)
	{};

	// bool addEffectJson(JsonObject& settings) override
	// {
	// 	Serial.printf("[CascadeEffect::addJson] Called\n");
	// 	Serial.println("[CascadeEffect::addJson] root");
	// 	settings.prettyPrintTo(Serial);
	// 	Serial.println();
	// }

//	bool parseJsonEffect(JsonObject& root) override;

	// bool testFn()
	// {
	// 	_color = RgbColor(0, 0, 0);
	// 	_brightness = 255;
	// }

private:
//	Palette _palette;

};


*/

//virtual bool parseJsonEffect(JsonObject & root) { return false;} // use json so it can be used with MQTT etc...
//virtual bool addEffectJson(JsonObject& settings) { return false; };

//	bool parseJsonEffectM(JsonObject & root) ;  // use json so it can be used with MQTT etc...
//	bool addEffectJsonM(JsonObject& root) ;


// class Effect2: public EffectHandler
// {
// public:
// 	Effect2()
// 	{
// 		_manager.addVar(new Variable<int>("int"));
// 		_manager.addVar(new Variable<RgbColor>("color1"));
// 		_manager.addVar(new Variable<RgbColor>("color2"));
// 		_manager.addVar(new Variable<RgbColor>("color3"));
// 		_manager.addVar(new Variable<RgbColor>("color4"));
// 		_manager.addVar(new Variable<RgbColor>("color5"));
// 	}

// 	//  required as inherited functions do not override
// 	bool addEffectJson(JsonObject & root) override  { return _manager.addEffectJsonM(root); }
// 	bool parseJsonEffect(JsonObject & root) override { return _manager.parseJsonEffectM(root); }

// 	void run()
// 	{
// 		int a = _manager.getVar<int>("int");
// 		RgbColor b = _manager.getVar<RgbColor>("color1");
// 	}
// private:
// 	PropertyManager _manager;
// };




