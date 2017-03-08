#include "mbed.h"
#include "utils.hpp"

#include "EthernetInterface.h"
#include "frdm_client.hpp"

#include "metronome.hpp"

#define IOT_ENABLED

namespace active_low
{
	const bool on = false;
	const bool off = true;
}

//DigitalOut g_led_red(LED1);
//DigitalOut g_led_green(LED2);
//DigitalOut g_led_blue(LED3);

InterruptIn g_button_mode(SW3);
InterruptIn g_button_tap(SW2);

metronome* globalMetronome = NULL;
bool learnMode = false;
bool playMode = true;
bool learning = false;
bool tickerEnabled = false;
bool tapNow =  false;
bool resetFlag = false;
Ticker playTicker;
int frdm_client::bpmPutValue = 0;
bool frdm_client::rFlag = false;

M2MResource* res1 = NULL;
M2MResource* res2 = NULL;
M2MResource* res3 = NULL;
M2MResource* res4 = NULL;
M2MResource* res5 = NULL;

static int MAX_BPM;
static int MIN_BPM;

void resetMINMAX(void*)
{
	MIN_BPM = 0;
	MAX_BPM = 0;
	char buffer[20];
	int size = sprintf(buffer,"%d",MIN_BPM);
	res2->set_value((const uint8_t*)buffer, size);
	size = sprintf(buffer,"%d",MAX_BPM);
	res3->set_value((const uint8_t*)buffer, size);
	printf("Reset MIN and MAX on POST operation\n");	
}


void play()
{	
	//printf("TICK\n");
	DigitalOut g_led(LED2);
	utils::pulse(g_led);
}


void on_mode()
{
    // Change modes
    learnMode = !learnMode;
  /*  if (!learnMode)
    	playMode = true;
    else
    	playMode = false;*/
}

void on_tap()
{
    // Receive a tempo tap
    if  (learnMode)
    {
    	//globalMetronome->tap();
    	//DigitalOut g_led(LED1);
    	//utils::pulse(g_led);
    	tapNow = true;
    }
}

int main()
{
	printf("MAIN function\n");
	// create metronome object
	globalMetronome = new metronome();
	// Seed the RNG for networking purposes
    unsigned seed = utils::entropy_seed();
    srand(seed);

	// LEDs are active LOW - true/1 means off, false/0 means on
	// Use the constants for easier reading
   // g_led_red = active_low::on;
   // g_led_green = active_low::on;
   // g_led_blue = active_low::on;

	// Button falling edge is on push (rising is on release)
    g_button_mode.fall(&on_mode);
    g_button_tap.fall(&on_tap);

#ifdef IOT_ENABLED
	// Turn on the blue LED until connected to the network
   // g_led_blue = active_low::off;

	// Need to be connected with Ethernet cable for success
    EthernetInterface ethernet;
    if (ethernet.connect() != 0)
        return 1;

	// Pair with the device connector
    frdm_client client("coap://api.connector.mbed.com:5684", &ethernet);
    if (client.get_state() == frdm_client::state::error)
        return 1;

	// The REST endpoints for this device
	// Add your own M2MObjects to this list with push_back before client.connect()
    M2MObjectList objects;

    M2MDevice* device = frdm_client::make_device();
    objects.push_back(device);

	// Publish the RESTful endpoints
	M2MObject* myObject = M2MInterfaceFactory::create_object("3318");
    M2MObjectInstance* myObject_instance = myObject->create_object_instance();

	
	// Resource for 5900
    res1 = myObject_instance->create_dynamic_resource("5900", "BPM", M2MResourceInstance::INTEGER, true);
    // read and write
    res1->set_operation(M2MBase::GET_PUT_ALLOWED);
	//myObject->set_observable(true);
	//res1->set_observable(true);
	
		//res1->set_incoming_block_message_callback(incoming_block_message_callback(&putBPM));
		
	char buffer[20];
	int size = sprintf(buffer,"%d",globalMetronome->get_bpm());
	res1->set_value((const uint8_t*)buffer, size);
	
	//res->set_execute_function(execute_callback(res,&putBPM));
	//res1->set_execute_function(&putBPM);

	// Resource object for 5601
	res2 = myObject_instance->create_dynamic_resource("5601", "BPM", M2MResourceInstance::FLOAT, true);
    // read only
    res2->set_operation(M2MBase::GET_ALLOWED);	
	//res2->set_observable(true);	
	// Minimum metronome BPM
	size = sprintf(buffer,"%d",MIN_BPM);
	res2->set_value((const uint8_t*)buffer, size);
	
	// Resource object for 5602
	res3 = myObject_instance->create_dynamic_resource("5602", "BPM", M2MResourceInstance::FLOAT, true);
    // read only
    res3->set_operation(M2MBase::GET_ALLOWED);	
	//res3->set_observable(true);	
	// Maximum metronome BPM
	size = sprintf(buffer,"%d",MAX_BPM);
	res3->set_value((const uint8_t*)buffer, size);
	
	// Resource for 5605
	res4 = myObject_instance->create_dynamic_resource("5605", "BPM", M2MResourceInstance::OPAQUE, true);
    // POST only
    res4->set_operation(M2MBase::POST_ALLOWED);		
	// HAndler fucntion for POST operation
	res4->set_execute_function(execute_callback(&resetMINMAX));
	// Resource for 5701
	//res5 = myObject_instance->create_dynamic_resource("5701", "BPM", M2MResourceInstance::STRING, false);
	res5 = myObject_instance->create_static_resource("5701", "BPM", M2MResourceInstance::STRING, (uint8_t*) "BPM", sizeof("BPM")-1);
    // read only
    res5->set_operation(M2MBase::GET_ALLOWED);	
	res5->set_observable(true);	
	// BPM string
	//size = sprintf(buffer,"%s","BPM");
	//res5->set_value((const char*)buffer, size);
	
	
	
	
	objects.push_back(myObject);
	
	
	
	
    client.connect(objects);

	// Connect complete; turn off blue LED forever
    //g_led_blue = active_low::on;
#endif
	printf("Enter while loop\n");
	frdm_client::bpmPutValue = globalMetronome->get_bpm();
    while (true)
    {
#ifdef IOT_ENABLED
        if (client.get_state() == frdm_client::state::error)
            break;
#endif



        // Insert any code that must be run continuously here
        
        // On receiving PUT after RESET and putting the same bpm value that exists
        if (playMode && frdm_client::bpmPutValue == globalMetronome->get_bpm() && frdm_client::rFlag && (MIN_BPM == 0 && MAX_BPM == 0))
        {
        	MAX_BPM = globalMetronome->get_bpm();
        	MIN_BPM = globalMetronome->get_bpm();
        	
        	size = sprintf(buffer,"%d",MIN_BPM);
			res2->set_value((const uint8_t*)buffer, size);
			printf("MIN_BPM is changed to %d\n",MIN_BPM);
			
			size = sprintf(buffer,"%d",MAX_BPM);
			res3->set_value((const uint8_t*)buffer, size);
			printf("MAX_BPM is changed to %d\n",MAX_BPM);
			frdm_client::rFlag = false;
        	
        }
        
        if (playMode && frdm_client::bpmPutValue != globalMetronome->get_bpm())
        {
        	globalMetronome->put_bpm(frdm_client::bpmPutValue);
        	playTicker.detach();
        	tickerEnabled = false;     
        	frdm_client::rFlag = false;  	
        }
        
        
        if (tapNow)
        {
	        globalMetronome->tap();
	    	DigitalOut g_led(LED1);
	    	utils::pulse(g_led);
	    	tapNow = false;
    	}
        
        if (learnMode && !learning)
        {
        	//printf("Learn mode \n");
        	playTicker.detach();
        	playMode = false;
        	globalMetronome->start_timing();
        	learning = true;
        }
        else if (!learnMode && learning)
        {
        	//printf("Learning \n");
        	globalMetronome->stop_timing();
        	learning = false;
        	tickerEnabled = false;
        	playMode = true;
        	//printf ("BPM value is %d\n",60/globalMetronome->get_bpm());
        }
        
        if (playMode && (globalMetronome->get_bpm() != 0) && !tickerEnabled)
        {
        		printf("Set Ticker \n");
        		// set BPM_MIN and BPM_MAX if required
        		if (globalMetronome->get_bpm() > MAX_BPM)
        		{
        			MAX_BPM = globalMetronome->get_bpm();
        			size = sprintf(buffer,"%d",MAX_BPM);
					res3->set_value((const uint8_t*)buffer, size);
					printf("MAX_BPM is changed to %d\n",MAX_BPM);
        		}
        		if (globalMetronome->get_bpm() < MIN_BPM && MIN_BPM > 0)
        		{
        			MIN_BPM = globalMetronome->get_bpm();
        			size = sprintf(buffer,"%d",MIN_BPM);
					res2->set_value((const uint8_t*)buffer, size);
					printf("MIN_BPM is changed to %d\n",MIN_BPM);
        		}
        		
        		if (MIN_BPM == 0 && globalMetronome->get_bpm() > 0)
        		{
        			MIN_BPM = globalMetronome->get_bpm();
        			size = sprintf(buffer,"%d",MIN_BPM);
					res2->set_value((const uint8_t*)buffer, size);
					printf("MIN_BPM is changed to %d\n",MIN_BPM);
        		}
        		
        
				size = sprintf(buffer,"%d",globalMetronome->get_bpm());
				res1->set_value((const uint8_t*)buffer, size);
				printf("BPM is changed to %d\n",globalMetronome->get_bpm());
        		frdm_client::bpmPutValue = globalMetronome->get_bpm();
        		printf("BPS is %f\n",(float)60/globalMetronome->get_bpm());
        		playTicker.attach(&play,(float)60/globalMetronome->get_bpm());
        		//playTicker.attach(&play,60/(globalMetronome->get_bpm()*1000));
        		tickerEnabled = true;        		
        }
        
        
        if (playMode && (globalMetronome->get_bpm() == 0) && !tickerEnabled)
        {
        	size = sprintf(buffer,"%d",globalMetronome->get_bpm());
			res1->set_value((const uint8_t*)buffer, size);
			printf("BPM is changed to %d\n",globalMetronome->get_bpm());   
			globalMetronome->put_bpm(frdm_client::bpmPutValue);        	
        	tickerEnabled = true;      	
        }
        //printf("DEBUG\n");
    }

#ifdef IOT_ENABLED
    client.disconnect();
#endif

    return 1;
}
