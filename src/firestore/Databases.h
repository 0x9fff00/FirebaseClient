/**
 * Created March 12, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef ASYNC_FIRESTORE_DATABASE_H
#define ASYNC_FIRESTORE_DATABASE_H
#include <Arduino.h>
#include "./core/FirebaseApp.h"
#include "./firestore/DataOptions.h"
#include "./firestore/FirestoreBase.h"

#if defined(ENABLE_FIRESTORE)

using namespace firebase;

#include "./firestore/Query.h"

namespace Firestore
{

    class Databases : public FirestoreBase
    {

    public:
        ~Databases() {}

        Databases() {}

        Databases &operator=(Databases &rhs)
        {
            return *this;
        }

        /** Export the documents in the database to the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param exportOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to export. Unspecified means all collections. Use comma (,)
         *   to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket to store the exported database.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool exportDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions exportOptions)
        {
            AsyncResult result;
            eximDocs(aClient, &result, NULL, "", parent, exportOptions, false, false);
            return result.lastError.code() == 0;
        }

        /** Export the documents in the database to the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param exportOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to export. Unspecified means all collections. Use comma (,)
         *   to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket to store the exported database.
         * @param aResult The async result (AsyncResult)
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void exportDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions exportOptions, AsyncResult &aResult)
        {
            eximDocs(aClient, &aResult, NULL, "", parent, exportOptions, false, true);
        }

        /** Export the documents in the database to the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param exportOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to export. Unspecified means all collections. Use comma (,)
         *   to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket to store the exported database.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void exportDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions exportOptions, AsyncResultCallback cb, const String &uid = "")
        {
            eximDocs(aClient, nullptr, cb, uid, parent, exportOptions, false, true);
        }

        /** Import the exported documents stored in the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param importOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to import. Unspecified means all collections included in the import.
         *   Use comma (,) to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket that stores the exported database.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool importDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions importOptions)
        {
            AsyncResult result;
            eximDocs(aClient, &result, NULL, "", parent, importOptions, true, false);
            return result.lastError.code() == 0;
        }

        /** Import the exported documents stored in the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param importOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to import. Unspecified means all collections included in the import.
         *   Use comma (,) to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket that stores the exported database.
         * @param aResult The async result (AsyncResult)
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void importDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions importOptions, AsyncResult &aResult)
        {
            eximDocs(aClient, &aResult, NULL, "", parent, importOptions, true, true);
        }

        /** Import the exported documents stored in the Firebase Storage data bucket.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database id should be (default) or empty "".
         * @param importOptions The EximDocumentOptions object included collectionIds, bucketID and storagePath in its constructor.
         * The EximDocumentOptions constructor parameters.
         * - collectionIds is collection ids to import. Unspecified means all collections included in the import.
         *   Use comma (,) to separate between the collection ids.
         * - bucketID is the Firebase storage bucket ID in the project.
         * - storagePath is the path in the Firebase Storage data bucket that stores the exported database.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void importDocuments(AsyncClientClass &aClient, const Firestore::Parent &parent, EximDocumentOptions importOptions, AsyncResultCallback cb, const String &uid = "")
        {
            eximDocs(aClient, nullptr, cb, uid, parent, importOptions, true, true);
        }

        /** Create a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to create.
         * @param database The Firestore::Database object that hold the database information to create.
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool create(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database)
        {
            AsyncResult result;
            manageDatabase(aClient, &result, NULL, "", parent, database.c_str(), "", Firestore::firestore_database_mode_create, false);
            return result.lastError.code() == 0;
        }

        /** Create a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to create.
         * @param database The Firestore::Database object that hold the database information to create.
         * @param aResult The async result (AsyncResult).
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void create(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database, AsyncResult &aResult)
        {
            manageDatabase(aClient, &aResult, NULL, "", parent, database.c_str(), "", Firestore::firestore_database_mode_create, true);
        }

        /** Create a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to create.
         * @param database The Firestore::Database object that hold the database information to create.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void create(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database, AsyncResultCallback cb, const String &uid = "")
        {
            manageDatabase(aClient, nullptr, cb, uid, parent, database.c_str(), "", Firestore::firestore_database_mode_create, true);
        }

        /** Deletes a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to delete.
         * @param etag The current etag of the Database.
         * If an etag is provided and does not match the current etag of the database, deletion will be blocked and a FAILED_PRECONDITION error will be returned.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool deleteDatabase(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &etag)
        {
            AsyncResult result;
            manageDatabase(aClient, &result, NULL, "", parent, "", etag, Firestore::firestore_database_mode_delete, false);
            return result.lastError.code() == 0;
        }

        /** Deletes a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to delete.
         * @param etag The current etag of the Database.
         * If an etag is provided and does not match the current etag of the database, deletion will be blocked and a FAILED_PRECONDITION error will be returned.
         * @param aResult The async result (AsyncResult).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void deleteDatabase(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &etag, AsyncResult &aResult)
        {
            manageDatabase(aClient, &aResult, NULL, "", parent, "", etag, Firestore::firestore_database_mode_delete, true);
        }

        /** Deletes a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to delete.
         * @param etag The current etag of the Database.
         * If an etag is provided and does not match the current etag of the database, deletion will be blocked and a FAILED_PRECONDITION error will be returned.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void deleteDatabase(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &etag, AsyncResultCallback cb, const String &uid = "")
        {
            manageDatabase(aClient, nullptr, cb, uid, parent, "", etag, Firestore::firestore_database_mode_delete, true);
        }

        /** Gets information about a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to get the infomation.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool get(AsyncClientClass &aClient, const Firestore::Parent &parent)
        {
            AsyncResult result;
            manageDatabase(aClient, &result, NULL, "", parent, "", "", Firestore::firestore_database_mode_get, false);
            return result.lastError.code() == 0;
        }

        /** Gets information about a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to get the infomation.
         * @param aResult The async result (AsyncResult).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void get(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResult &aResult)
        {
            manageDatabase(aClient, &aResult, NULL, "", parent, "", "", Firestore::firestore_database_mode_get, true);
        }

        /** Gets information about a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to get the infomation.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void get(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResultCallback cb, const String &uid = "")
        {
            manageDatabase(aClient, nullptr, cb, uid, parent, "", "", Firestore::firestore_database_mode_get, true);
        }

        /** List all the databases in the project.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * Leave the Firestore database Id to be empty for this case.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool list(AsyncClientClass &aClient, const Firestore::Parent &parent)
        {
            AsyncResult result;
            manageDatabase(aClient, &result, NULL, "", parent, "", "", Firestore::firestore_database_mode_list, false);
            return result.lastError.code() == 0;
        }

        /** List all the databases in the project.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * Leave the Firestore database Id to be empty for this case.
         * @param aResult The async result (AsyncResult).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void list(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResult &aResult)
        {
            manageDatabase(aClient, &aResult, NULL, "", parent, "", "", Firestore::firestore_database_mode_list, true);
        }

        /** List all the databases in the project.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * Leave the Firestore database Id to be empty for this case.
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void list(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResultCallback cb, const String &uid = "")
        {
            manageDatabase(aClient, nullptr, cb, uid, parent, "", "", Firestore::firestore_database_mode_list, true);
        }

        /** Updates a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to update.
         * @param database The Firestore::Database object that hold the database information to update.
         * @param updateMask The list of fields to be updated. This is a comma-separated list of fully qualified names of fields.
         * Example: "user.displayName,photo"
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * @return Boolean value, indicates the success of the operation.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        bool patch(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database, const String &updateMask)
        {
            AsyncResult result;
            manageDatabase(aClient, &result, NULL, "", parent, database.c_str(), updateMask, Firestore::firestore_database_mode_patch, false);
            return result.lastError.code() == 0;
        }

        /** Updates a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to update.
         * @param database The Firestore::Database object that hold the database information to update.
         * @param updateMask The list of fields to be updated. This is a comma-separated list of fully qualified names of fields.
         * Example: "user.displayName,photo"
         * @param aResult The async result (AsyncResult).
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void patch(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database, const String &updateMask, AsyncResult &aResult)
        {
            manageDatabase(aClient, &aResult, NULL, "", parent, database.c_str(), updateMask, Firestore::firestore_database_mode_patch, true);
        }

        /** Updates a database.
         *
         * @param aClient The async client.
         * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
         * The Firebase project Id should be only the name without the firebaseio.com.
         * The Firestore database Id is the Id of database to update.
         * @param database The Firestore::Database object that hold the database information to update.
         * @param updateMask The list of fields to be updated. This is a comma-separated list of fully qualified names of fields.
         * Example: "user.displayName,photo"
         * @param cb The async result callback (AsyncResultCallback).
         * @param uid The user specified UID of async result (optional).
         *
         * The Database information should be set via the following functions befor creation.
         * concurrencyMode, appEngineIntegrationMode, deleteProtectionState, pointInTimeRecoveryEnablement, databaseType, locationId and name.
         *
         * This function requires ServiceAuth or AccessToken authentication.
         *
         */
        void patch(AsyncClientClass &aClient, const Firestore::Parent &parent, Database &database, const String &updateMask, AsyncResultCallback cb, const String &uid = "")
        {
            manageDatabase(aClient, nullptr, cb, uid, parent, database.c_str(), updateMask, Firestore::firestore_database_mode_patch, true);
        }

        class Indexes : public FirestoreBase
        {
        public:
            Indexes() {}

            /** Creates the specified index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param index The Index object that provides an index definition.
             *
             * @return Boolean value, indicates the success of the operation.
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/create
             *
             * This function requires ServiceAuth authentication.
             *
             */
            bool create(AsyncClientClass &aClient, const Firestore::Parent &parent, DatabaseIndex::Index index)
            {
                AsyncResult result;
                databaseIndexManager(aClient, &result, NULL, "", parent, index, "", false, false);
                return result.lastError.code() == 0;
            }

            /** Creates the specified index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param index The Index object that provides an index definition.
             * @param aResult The async result (AsyncResult).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/create
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void create(AsyncClientClass &aClient, const Firestore::Parent &parent, DatabaseIndex::Index index, AsyncResult &aResult)
            {
                databaseIndexManager(aClient, &aResult, NULL, "", parent, index, "", false, true);
            }

            /** Creates the specified index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param index The Index object that provides an index definition.
             * @param cb The async result callback (AsyncResultCallback).
             * @param uid The user specified UID of async result (optional).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/create
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void create(AsyncClientClass &aClient, const Firestore::Parent &parent, DatabaseIndex::Index index, AsyncResultCallback cb, const String &uid = "")
            {
                databaseIndexManager(aClient, nullptr, cb, uid, parent, index, "", false, true);
            }

            /** Deletes an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to delete.
             *
             * @return Boolean value, indicates the success of the operation.
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/delete
             *
             * This function requires ServiceAuth authentication.
             *
             */
            bool deleteIndex(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId)
            {
                AsyncResult result;
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &result, NULL, "", parent, index, indexId, true, false);
                return result.lastError.code() == 0;
            }

            /** Deletes an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to delete.
             * @param aResult The async result (AsyncResult).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/delete
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void deleteIndex(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId, AsyncResult &aResult)
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &aResult, NULL, "", parent, index, indexId, true, true);
            }

            /** Deletes an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to delete.
             * @param cb The async result callback (AsyncResultCallback).
             * @param uid The user specified UID of async result (optional).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/delete
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void deleteIndex(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId, AsyncResultCallback cb, const String &uid = "")
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, nullptr, cb, uid, parent, index, indexId, true, true);
            }

            /** Gets an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to get.
             *
             * @return Boolean value, indicates the success of the operation.
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/get
             *
             * This function requires ServiceAuth authentication.
             *
             */
            bool get(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId)
            {
                AsyncResult result;
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &result, NULL, "", parent, index, indexId, false, false);
                return result.lastError.code() == 0;
            }

            /** Gets an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to get.
             * @param aResult The async result (AsyncResult).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/get
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void get(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId, AsyncResult &aResult)
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &aResult, NULL, "", parent, index, indexId, false, true);
            }

            /** Gets an index.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to get.
             * @param cb The async result callback (AsyncResultCallback).
             * @param uid The user specified UID of async result (optional).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/get
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void get(AsyncClientClass &aClient, const Firestore::Parent &parent, const String &indexId, AsyncResultCallback cb, const String &uid = "")
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, nullptr, cb, uid, parent, index, indexId, false, true);
            }

            /** Lists the indexes.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             *
             * @return Boolean value, indicates the success of the operation.
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/list
             *
             * This function requires ServiceAuth authentication.
             *
             */
            bool list(AsyncClientClass &aClient, const Firestore::Parent &parent)
            {
                AsyncResult result;
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &result, NULL, "", parent, index, "", false, false);
                return result.lastError.code() == 0;
            }

            /** Lists the indexes.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param indexId The index to get.
             * @param aResult The async result (AsyncResult).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/list
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void list(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResult &aResult)
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, &aResult, NULL, "", parent, index, "", false, true);
            }

            /** Lists the indexes.
             *
             * @param aClient The async client.
             * @param parent The Firestore::Parent object included project Id and database Id in its constructor.
             * The Firebase project Id should be only the name without the firebaseio.com.
             * The Firestore database id should be (default) or empty "".
             * @param cb The async result callback (AsyncResultCallback).
             * @param uid The user specified UID of async result (optional).
             *
             * For more description, see https://firebase.google.com/docs/firestore/reference/rest/v1beta1/projects.databases.indexes/list
             *
             * This function requires ServiceAuth authentication.
             *
             */
            void list(AsyncClientClass &aClient, const Firestore::Parent &parent, AsyncResultCallback cb, const String &uid = "")
            {
                DatabaseIndex::Index index("");
                databaseIndexManager(aClient, nullptr, cb, uid, parent, index, "", false, true);
            }
        };
    };

}

#endif

#endif