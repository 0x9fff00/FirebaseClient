
// Created by K. Suwatchai (Mobizt)
// Email: k_suwatchai@hotmail.com
// Github: https://github.com/mobizt/FirebaseClient
// Copyright (c) 2024 mobizt

/**
 * APP INITIALIZATION
 * ==================
 *
 * This library provides the APIs covers the different types of authentication and services.
 *
 * The FirebaseApp class was introduced as user authentication provider which provides the credentials data
 * used for accessing the Firebase services.
 *
 * The Database, Firestore, Functions, Messaging, Storage and Cloud Storage apps provided the functions based on
 * the specific Firebase services which take the auth credentials data from FirebaseApp.
 *
 * Multiples instances of FirebaseApp (auth provider) and services apps (Database, Firestore...) can be available
 * which depends on the device memory.
 *
 * The FirebaseApp should be initialized first before use via static function initializeApp.
 *
 * SYNTAXES:
 *
 * initializeApp(<AsyncClient>, <FirebaseApp>, <user_auth_data>);
 *
 * The AsyncClient stores the user, server and network data which can be initialized as.
 *
 * WiFiClientSecure ssl_client;
 * DefaultNetwork network;
 * AsyncClient aClient(ssl_client, getNetwork(network));
 *
 * The FirebaseApp manage and stores the authentication data;
 *
 * The user_auth_data store the user credentials which can be obtained from one of following Auth classes.
 * UserAuth, ServiceAuth, CustomAuth, IDToken, AccessToken, CustomToken, LegacyToken and NoAuth via static function getAuth.
 *
 * Please see examples/App/AppInitialization for usage guidelines.
 *
 * To use other network interfaces, network data from one of the following Network classes
 * can be assigned.
 *
 * DefaultNetwork, GenericNetwork, EthernetNetwork and GSMNetwork.
 *
 * The network data (network_config_data) can be obtained from these Network classes via static function getNetwork.
 *
 * Please see examples/App/NetworkInterfaces for the usage guidelines.
 *
 * The auth credential data can be set to Services App e.g. Database to initialize via function getApp.
 *
 * SYNTAX:
 *
 * app.getApp<T>(<services_app>);
 *
 * Deallocation of FirebaseApp causes these services apps uninitialized and cannot be used.
 *
 */

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <FirebaseClient.h>

#if defined(ESP8266) || defined(ESP32)
#include <WiFiClientSecure.h>
#endif

#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// The API key can be obtained from Firebase console > Project Overview > Project settings.
#define API_KEY "Web_API_KEY"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"
#define DATABASE_URL "URL"

void asyncCB(AsyncResult &aResult);

void printError(int code, const String &msg);

DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;

WiFiClientSecure ssl_client;

// In case the keyword AsyncClient using in this example was ambigous and used by other library, you can change
// it with other name with keyword "using" or use the class name AsyncClientClass directly.

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

void setup()
{

    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    Serial.println("Initializing app...");

    ssl_client.setInsecure();
#if defined(ESP8266)
    ssl_client.setBufferSizes(4096, 1024);
#endif

    app.setCallback(asyncCB);

    initializeApp(aClient, app, getAuth(user_auth));

    // Waits for app to be authenticated.
    // For asynchronous operation, this blocking wait can be ignored by calling app.loop() in loop().
    ms = millis();
    while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
        ;

    app.getApp<RealtimeDatabase>(Database);

    Database.url(DATABASE_URL);

    Serial.println();

    Serial.println("[+] Set data with priority (sync)... ");

    // Library does not provide JSON parser library, the following JSON writer class will be used with
    // object_t for simple demonstration.
    object_t json, obj1, obj2;
    JsonWriter writer;

    for (int i = 0; i < 15; i++)
    {
        // Set priority to object or node that contains children.
        float priority = 15 - i;

        writer.create(obj1, "item_" + String(i + 1), string_t("value_" + String(i + 1))); // -> {"item_x":"value_x"}
        writer.create(obj2, ".priority", string_t(priority));                             // -> {".priority":x}
        writer.join(json, 2, obj1, obj2);

        String path = "/test/items/priority_" + String(15 - i);

        bool status = Database.set<object_t>(aClient, path, json);
        if (status)
            Serial.println(String("ok"));
        else
            printError(aClient.lastError().code(), aClient.lastError().message());
    }

    // Change priority
    Serial.println();
    Serial.println("[+] Set value with priority (sync)... ");

    writer.create(obj1, ".value", "item_15");
    // Set priority to primitive value.
    writer.create(obj2, ".priority", 6.0);
    writer.join(json, 2, obj1, obj2);

    bool status = Database.set<object_t>(aClient, "/test/items/priority_1", json);
    if (status)
        Serial.println(String("ok"));
    else
        printError(aClient.lastError().code(), aClient.lastError().message());

    Serial.println();
    Serial.println("[+] Get priority (sync)... ");
    double v = Database.get<double>(aClient, "/test/items/priority_1/.priority");

    if (aClient.lastError().code() == 0)
        Serial.println(String(v));
    else
        printError(aClient.lastError().code(), aClient.lastError().message());

    Serial.println();
    Serial.println("[+] Filtering data with priority (sync)... ");
    DatabaseOptions options;
    // Now the node priority_1 which its priority changed to 6.0 should include in the filtering result.
    options.filter.orderBy("$priority").startAt(3.0).endAt(8.0);
    Database.get(aClient, "/test/items", options, asyncCB);
}

void loop()
{
    // This function is required for handling async operations and maintaining the authentication tasks.
    app.loop();

    // This required when different AsyncClients than used in FirebaseApp assigned to the Realtime database functions.
    Database.loop();
}

void asyncCB(AsyncResult &aResult)
{
    if (aResult.appEvent().code() > 0)
    {
        Serial.println("**************");
        Serial.printf("Event msg: %s, code: %d\n", aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isError())
    {
        Serial.println("**************");
        Serial.printf("Error msg: %s, code: %d\n", aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.isDebug())
    {
        Serial.println("**************");
        Serial.printf("Debug msg: %s\n", aResult.debug().c_str());
    }

    if (aResult.available())
    {
        Serial.println("**************");
        Serial.printf("payload: %s\n", aResult.c_str());
    }
}

void printError(int code, const String &msg)
{
    Serial.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}