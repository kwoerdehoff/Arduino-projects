/*
 * config.h
 *
 *  Created on: Dec 5, 2013
 *      Author: swood
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// serial debugging (may be needed by ethernet library)

// ethernet shield
#ifndef ETHERCONF


#define ETHERCONF true
#endif


// current sensor
#ifndef ACS714PIN
#define ACS714PIN 0 // analogue pin 0
#endif


// water level switch
#ifndef WATERSENSORPIN
#define WATERSENSORPIN 1
#endif


// temp and humidity sensor
#ifndef DHT11PIN
#define DHT11PIN 2
#endif


// gas sensor
#ifndef MQDPIN
#define MQDPIN 3
#endif

#endif /* CONFIG_H_ */
