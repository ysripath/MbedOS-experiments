/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SECURITY_H__
#define __SECURITY_H__
 
#include <inttypes.h>
 
#define MBED_DOMAIN "1fb1fe3a-3859-4c05-bcc2-80a81f15af19"
#define MBED_ENDPOINT_NAME "dc3a4c76-aeca-4f18-8913-86809174c023"
 
const uint8_t SERVER_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBmDCCAT6gAwIBAgIEVUCA0jAKBggqhkjOPQQDAjBLMQswCQYDVQQGEwJGSTEN\r\n"
"MAsGA1UEBwwET3VsdTEMMAoGA1UECgwDQVJNMQwwCgYDVQQLDANJb1QxETAPBgNV\r\n"
"BAMMCEFSTSBtYmVkMB4XDTE1MDQyOTA2NTc0OFoXDTE4MDQyOTA2NTc0OFowSzEL\r\n"
"MAkGA1UEBhMCRkkxDTALBgNVBAcMBE91bHUxDDAKBgNVBAoMA0FSTTEMMAoGA1UE\r\n"
"CwwDSW9UMREwDwYDVQQDDAhBUk0gbWJlZDBZMBMGByqGSM49AgEGCCqGSM49AwEH\r\n"
"A0IABLuAyLSk0mA3awgFR5mw2RHth47tRUO44q/RdzFZnLsAsd18Esxd5LCpcT9w\r\n"
"0tvNfBv4xJxGw0wcYrPDDb8/rjujEDAOMAwGA1UdEwQFMAMBAf8wCgYIKoZIzj0E\r\n"
"AwIDSAAwRQIhAPAonEAkwixlJiyYRQQWpXtkMZax+VlEiS201BG0PpAzAiBh2RsD\r\n"
"NxLKWwf4O7D6JasGBYf9+ZLwl0iaRjTjytO+Kw==\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBzzCCAXOgAwIBAgIEbWm+RzAMBggqhkjOPQQDAgUAMDkxCzAJBgNVBAYTAkZ\r\n"
"JMQwwCgYDVQQKDANBUk0xHDAaBgNVBAMME21iZWQtY29ubmVjdG9yLTIwMTgwHh\r\n"
"cNMTcwMjE5MjIzNjUwWhcNMTgxMjMxMDYwMDAwWjCBoTFSMFAGA1UEAxNJMWZiM\r\n"
"WZlM2EtMzg1OS00YzA1LWJjYzItODBhODFmMTVhZjE5L2RjM2E0Yzc2LWFlY2Et\r\n"
"NGYxOC04OTEzLTg2ODA5MTc0YzAyMzEMMAoGA1UECxMDQVJNMRIwEAYDVQQKEwl\r\n"
"tYmVkIHVzZXIxDTALBgNVBAcTBE91bHUxDTALBgNVBAgTBE91bHUxCzAJBgNVBA\r\n"
"YTAkZJMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE8/SIkrUBKHcGIJ5+GBpId\r\n"
"sMs72NOmbGbO7FKak+38Z7hlaGEHF5etFTRKsWdPj5NULiAHUqAOVz6MfMU7uOV\r\n"
"PzAMBggqhkjOPQQDAgUAA0gAMEUCIQD0QoaaI5veiSdmGWGMZdx3xc66Az+BLZ1\r\n"
"ajcmjwPSc+gIgRK3wJvJCQz2VLunQByf/cfn8jM0roXIdI+935bs/mdc=\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t KEY[] = "-----BEGIN PRIVATE KEY-----\r\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgNVONHoLomjnNcilE\r\n"
"XuH4Ez/atJm8xGf251g3mY40VR+hRANCAATz9IiStQEodwYgnn4YGkh2wyzvY06Z\r\n"
"sZs7sUpqT7fxnuGVoYQcXl60VNEqxZ0+Pk1QuIAdSoA5XPox8xTu45U/\r\n"
"-----END PRIVATE KEY-----\r\n";
 
#endif //__SECURITY_H__