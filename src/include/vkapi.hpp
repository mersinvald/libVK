/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

#ifndef VKAPI_VKAPI_H
#define VKAPI_VKAPI_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <map>
#include <exception>
#include <curl/curl.h>
#include <chrono>

#include "types.hpp"

namespace vk {
using std::chrono::milliseconds;

enum VKResultCode_t {
    RESULT_SUCCESS                          = 0,

    RESULT_ERROR                            = 1,
    RESULT_APPLICATION_DISABLED             = 2,
    RESULT_UNKNOWN_METHOD                   = 3,
    RESULT_INVALID_SIGNATURE                = 4,
    RESULT_AUTORIZATION_ERROR               = 5,
    RESULT_TOO_MANY_REQUESTS                = 6,
    RESULT_NOT_PERMITED                     = 7,
    RESULT_INVALID_REQUEST                  = 8,
    RESULT_TOO_MANY_SIMILAR_REQUESTS        = 9,
    RESULT_INTERNAL_ERROR                   = 10,
    RESULT_APPLICATION_MUST_BE_DISABLED     = 11,
    RESULT_CAPTHA                           = 14,
    RESULT_FORBIDDEN                        = 15,
    RESULT_HTTPS_REQUIRED                   = 16,
    RESULT_VALIDATION_REQUIRED              = 17,
    RESULT_ONLY_STANDALONE                  = 20,
    RESULT_ONLY_STANDALONE_AND_OPENAPI      = 21,
    RESULT_METHOD_DISABLED                  = 23,
    RESULT_CONFIRMATION_NEEDED              = 24,
    RESULT_INVALID_PARAMETERS               = 100,
    RESULT_INVALID_APPID                    = 101,
    RESULT_INVALID_IDENTIFIER               = 113,
    RESULT_INVALID_TIMESTAMP                = 150,
    RESULT_ACCESS_FORBIDDEN_ALBUM           = 200,
    RESILT_ACCESS_FORBIDDEN_AUDIO           = 201,
    RESULT_ACCESS_FORBIDDEN_GROUP           = 203,
    RESULT_ALBUM_OVERFLOWED                 = 300,
    RESULT_VOTE_TRANSACTIONS_DISABLED       = 500,
    RESULT_ADVERTISMENT_OPERATION_FORBIDDEN = 600,
    RESULT_ADVERTISMENT_OPERATION_ERROR     = 603,
};

/// Error handling
extern void printStackTrace(uint16_t max_frames, bool forced = false);
class libVKException : public std::exception {
public:
    using std::exception::exception;
    virtual ~libVKException() noexcept {}
    libVKException();
    libVKException(int err_code, const string& error_msg);
    libVKException(const char*   error_msg);
    libVKException(const string& error_msg);

    friend std::ostream& operator<< (std::ostream& os, const libVKException& e);
    const char* what() const noexcept;

    string  err_str;
    int     err_code;
};

struct VKException : public libVKException {
public:
    using libVKException::libVKException;
    explicit VKException(const VKValue& json);
};

/// Concrete exceptions for non-vk api errors
struct CurlException : public libVKException { using libVKException::libVKException; };
struct JsonException : public libVKException { using libVKException::libVKException; };

#define VKAPI_URL       "https://api.vk.com/method/"
#define VKAPI_AUTH_URL  "https://oauth.vk.com/"

#define API_SUBCLASS_INIT(name) \
    private: VKAPI* this_ptr; \
    public:  name (VKAPI* ptr) : this_ptr(ptr) {}

#define API_METHOD_ARGS                       Args& args
#define API_SUBCLASS_METHOD_REQUEST(method) { return this_ptr->Request((method), args); }
#define API_METHOD_REQUEST(method)          { return           Request((method), args); }
#define API_RETURN_VALUE                      VKValue

class VKAPI {
public:
    VKAPI();
    VKAPI(const string& app_id, const string& app_secret);

    /* Base functionality */

    API_RETURN_VALUE Authorize(const string& login, const string& passwd, string* access_token = NULL);
    API_RETURN_VALUE Request(const string& method, Args& arguments);

    /* Setters */

    void SetAppID             (const string& app_id);
    void SetAppSecret         (const string& app_secret);
    void SetDefaultLang       (const string& lang);
    void SetDefaultAccessToken(const string& token);
    void SetDefaultAPIVersion (const string& version);
    void SetMaxRequestsPerSec (const uint8_t max_requests);

    /* Getters */

    CURLcode       getCurlError()   const;
    VKResultCode_t getVKError()     const;
    const VKValue& getJSON()        const;
    const string&  getAccessToken() const;

    /* API methods */
    inline API_RETURN_VALUE queue      (API_METHOD_ARGS);
    inline API_RETURN_VALUE execute_vk (API_METHOD_ARGS)                    API_METHOD_REQUEST("execute")

    class users_api {
        API_SUBCLASS_INIT(users_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("users.get")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                  	API_SUBCLASS_METHOD_REQUEST("users.search")
        inline API_RETURN_VALUE isAppUser (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("users.isAppUser")
        inline API_RETURN_VALUE getSubscriptions (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("users.getSubscriptions")
        inline API_RETURN_VALUE getFollowers (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("users.getFollowers")
        inline API_RETURN_VALUE report (API_METHOD_ARGS)                  	API_SUBCLASS_METHOD_REQUEST("users.report")
        inline API_RETURN_VALUE getNearby (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("users.getNearby")
    } users;

    class auth_api {
        API_SUBCLASS_INIT(auth_api)
        inline API_RETURN_VALUE checkPhone (API_METHOD_ARGS)          		API_SUBCLASS_METHOD_REQUEST("auth.checkPhone")
        inline API_RETURN_VALUE signup (API_METHOD_ARGS)              		API_SUBCLASS_METHOD_REQUEST("auth.signup")
        inline API_RETURN_VALUE confirm (API_METHOD_ARGS)             		API_SUBCLASS_METHOD_REQUEST("auth.confirm")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)             		API_SUBCLASS_METHOD_REQUEST("auth.restore")
    } auth;

    class wall_api {
        API_SUBCLASS_INIT(wall_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                 		API_SUBCLASS_METHOD_REQUEST("wall.get")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)          			API_SUBCLASS_METHOD_REQUEST("wall.search")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("wall.getById")
        inline API_RETURN_VALUE post (API_METHOD_ARGS)                		API_SUBCLASS_METHOD_REQUEST("wall.post")
        inline API_RETURN_VALUE repost (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("wall.repost")
        inline API_RETURN_VALUE getReposts (API_METHOD_ARGS)              	API_SUBCLASS_METHOD_REQUEST("wall.getReposts")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("wall.edit")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("wall.del")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("wall.restore")
        inline API_RETURN_VALUE pin (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("wall.pin")
        inline API_RETURN_VALUE unpin (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("wall.unpin")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)               API_SUBCLASS_METHOD_REQUEST("wall.getComments")
        inline API_RETURN_VALUE addComment (API_METHOD_ARGS)                API_SUBCLASS_METHOD_REQUEST("wall.addComment")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)               API_SUBCLASS_METHOD_REQUEST("wall.editComment")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)                API_SUBCLASS_METHOD_REQUEST("wall.delComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)            API_SUBCLASS_METHOD_REQUEST("wall.restoreComment")
        inline API_RETURN_VALUE reportPost (API_METHOD_ARGS)                API_SUBCLASS_METHOD_REQUEST("wall.reportPost")
        inline API_RETURN_VALUE reportComment (API_METHOD_ARGS)             API_SUBCLASS_METHOD_REQUEST("wall.reportComment")
    } wall;

    class photos_api {
        API_SUBCLASS_INIT(photos_api)
        inline API_RETURN_VALUE createAlbum (API_METHOD_ARGS)         		API_SUBCLASS_METHOD_REQUEST("photos.createAlbum")
        inline API_RETURN_VALUE editAlbum (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("photos.editAlbum")
        inline API_RETURN_VALUE getAlbums (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("photos.getAlbums")
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("photos.get")
        inline API_RETURN_VALUE getAlbumsCount (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("photos.getAlbumsCount")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("photos.getById")
        inline API_RETURN_VALUE getUploadServer (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("photos.getUploadServer")
        inline API_RETURN_VALUE getOwnerPhotoUploadServer (API_METHOD_ARGS)	API_SUBCLASS_METHOD_REQUEST("photos.getOwnerPhotoUploadServer")
        inline API_RETURN_VALUE getChatUploadServer (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("photos.getChatUploadServer")
        inline API_RETURN_VALUE getMarketUploadServer (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("photos.getMarketUploadServer")
        inline API_RETURN_VALUE getMarketAlbumUploadServer (API_METHOD_ARGS)API_SUBCLASS_METHOD_REQUEST("photos.getMarketAlbumUploadServer")
        inline API_RETURN_VALUE saveMarketPhoto (API_METHOD_ARGS)           API_SUBCLASS_METHOD_REQUEST("photos.saveMarketPhoto")
        inline API_RETURN_VALUE saveMarketAlbumPhoto (API_METHOD_ARGS)    	API_SUBCLASS_METHOD_REQUEST("photos.saveMarketAlbumPhoto")
        inline API_RETURN_VALUE saveOwnerPhoto (API_METHOD_ARGS)          	API_SUBCLASS_METHOD_REQUEST("photos.saveOwnerPhoto")
        inline API_RETURN_VALUE saveWallPhoto (API_METHOD_ARGS)             API_SUBCLASS_METHOD_REQUEST("photos.saveWallPhoto")
        inline API_RETURN_VALUE getWallUploadServer (API_METHOD_ARGS)       API_SUBCLASS_METHOD_REQUEST("photos.getWallUploadServer")
        inline API_RETURN_VALUE getMessagesUploadServer (API_METHOD_ARGS) 	API_SUBCLASS_METHOD_REQUEST("photos.getMessagesUploadServer")
        inline API_RETURN_VALUE saveMessagesPhoto (API_METHOD_ARGS)         API_SUBCLASS_METHOD_REQUEST("photos.saveMessagesPhoto")
        inline API_RETURN_VALUE report (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("photos.report")
        inline API_RETURN_VALUE reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reportComment")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("photos.search")
        inline API_RETURN_VALUE save (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.save")
        inline API_RETURN_VALUE copy (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.copy")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.edit")
        inline API_RETURN_VALUE move (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.move")
        inline API_RETURN_VALUE makeCover (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.makeCover")
        inline API_RETURN_VALUE reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reorderAlbums")
        inline API_RETURN_VALUE reorderPhotos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reorderPhotos")
        inline API_RETURN_VALUE getAll (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("photos.getAll")
        inline API_RETURN_VALUE getUserPhotos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.getUserPhotos")
        inline API_RETURN_VALUE delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.delAlbum")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("photos.del")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("photos.restore")
        inline API_RETURN_VALUE confirmTag (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.confirmTag")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.getComments")
        inline API_RETURN_VALUE getAllComments (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("photos.getAllComments")
        inline API_RETURN_VALUE createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.createComment")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.delComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("photos.restoreComment")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.editComment")
        inline API_RETURN_VALUE getTags (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("photos.getTags")
        inline API_RETURN_VALUE putTag (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("photos.putTag")
        inline API_RETURN_VALUE removeTag (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.removeTag")
        inline API_RETURN_VALUE getNewTags (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.getNewTags")
    } photos;

    class friends_api {
        API_SUBCLASS_INIT(friends_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("friends.get")
        inline API_RETURN_VALUE getOnline (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getOnline")
        inline API_RETURN_VALUE getMutual (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getMutual")
        inline API_RETURN_VALUE getRecent (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getRecent")
        inline API_RETURN_VALUE getRequests (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("friends.getRequests")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("friends.add")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("friends.edit")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("friends.del")
        inline API_RETURN_VALUE getLists (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getLists")
        inline API_RETURN_VALUE addList (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("friends.addList")
        inline API_RETURN_VALUE editList (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.editList")
        inline API_RETURN_VALUE delList (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("friends.delList")
        inline API_RETURN_VALUE getAppUsers (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("friends.getAppUsers")
        inline API_RETURN_VALUE getByPhones (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("friends.getByPhones")
        inline API_RETURN_VALUE delAllRequests (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("friends.delAllRequests")
        inline API_RETURN_VALUE getSuggestions (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("friends.getSuggestions")
        inline API_RETURN_VALUE areFriends (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("friends.areFriends")
        inline API_RETURN_VALUE getAvailableForCall (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("friends.getAvailableForCall")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("friends.search")
    } friends;

    class widgets_api {
        API_SUBCLASS_INIT(widgets_api)
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("widgets.getComments")
        inline API_RETURN_VALUE getPages (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("widgets.getPages")
    } widgets;

    class storage_api {
        API_SUBCLASS_INIT(storage_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("storage.get")
        inline API_RETURN_VALUE set (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("storage.set")
        inline API_RETURN_VALUE getKeys (API_METHOD_ARGS)         		    API_SUBCLASS_METHOD_REQUEST("storage.getKeys")
    } storage;

    class status_api {
        API_SUBCLASS_INIT(status_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)         			    API_SUBCLASS_METHOD_REQUEST("status.get")
        inline API_RETURN_VALUE set (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("status.set")
    } status;

    class audio_api {
        API_SUBCLASS_INIT(audio_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("audio.get")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("audio.getById")
        inline API_RETURN_VALUE getLyrics (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.getLyrics")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("audio.search")
        inline API_RETURN_VALUE getUploadServer (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("audio.getUploadServer")
        inline API_RETURN_VALUE save (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("audio.save")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("audio.add")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("audio.del")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("audio.edit")
        inline API_RETURN_VALUE reorder (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("audio.reorder")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("audio.restore")
        inline API_RETURN_VALUE getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.getAlbums")
        inline API_RETURN_VALUE addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.addAlbum")
        inline API_RETURN_VALUE editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.editAlbum")
        inline API_RETURN_VALUE delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.delAlbum")
        inline API_RETURN_VALUE moveToAlbum (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("audio.moveToAlbum")
        inline API_RETURN_VALUE setBroadcast (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("audio.setBroadcast")
        inline API_RETURN_VALUE getBroadcastList (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("audio.getBroadcastList")
        inline API_RETURN_VALUE getRecommendations (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("audio.getRecommendations")
        inline API_RETURN_VALUE getPopular (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("audio.getPopular")
        inline API_RETURN_VALUE getCount (API_METHOD_ARGS)                  API_SUBCLASS_METHOD_REQUEST("audio.getCount")
    } audio;

    class pages_api {
        API_SUBCLASS_INIT(pages_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("pages.get")
        inline API_RETURN_VALUE save (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("pages.save")
        inline API_RETURN_VALUE saveAccess (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("pages.saveAccess")
        inline API_RETURN_VALUE getHistory (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("pages.getHistory")
        inline API_RETURN_VALUE getTitles (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.getTitles")
        inline API_RETURN_VALUE getVersion (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("pages.getVersion")
        inline API_RETURN_VALUE parseWiki (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.parseWiki")
        inline API_RETURN_VALUE clearCache (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("pages.clearCache")
    } pages;

    class groups_api {
        API_SUBCLASS_INIT(groups_api)
        inline API_RETURN_VALUE isMember (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.isMember")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("groups.getById")
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("groups.get")
        inline API_RETURN_VALUE getMembers (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.getMembers")
        inline API_RETURN_VALUE join (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("groups.join")
        inline API_RETURN_VALUE leave (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("groups.leave")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("groups.search")
        inline API_RETURN_VALUE getCatalog (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.getCatalog")
        inline API_RETURN_VALUE getCatalogInfo (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("groups.getCatalogInfo")
        inline API_RETURN_VALUE getInvites (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.getInvites")
        inline API_RETURN_VALUE getInvitedUsers (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("groups.getInvitedUsers")
        inline API_RETURN_VALUE banUser (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("groups.banUser")
        inline API_RETURN_VALUE unbanUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.unbanUser")
        inline API_RETURN_VALUE getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getBanned")
        inline API_RETURN_VALUE create (API_METHOD_ARGS)                 	API_SUBCLASS_METHOD_REQUEST("groups.create")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("groups.edit")
        inline API_RETURN_VALUE editPlace (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.editPlace")
        inline API_RETURN_VALUE getSettings (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.getSettings")
        inline API_RETURN_VALUE getRequests (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.getRequests")
        inline API_RETURN_VALUE editManager (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.editManager")
        inline API_RETURN_VALUE invite (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("groups.invite")
        inline API_RETURN_VALUE addLink (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("groups.addLink")
        inline API_RETURN_VALUE delLink (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("groups.delLink")
        inline API_RETURN_VALUE editLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.editLink")
        inline API_RETURN_VALUE reorderLink (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.reorderLink")
        inline API_RETURN_VALUE removeUser (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("groups.removeUser")
        inline API_RETURN_VALUE approveRequest (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("groups.approveRequest")
    } groups;

    class board_api {
        API_SUBCLASS_INIT(board_api)
        inline API_RETURN_VALUE getTopics (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.getTopics")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.getComments")
        inline API_RETURN_VALUE addTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.addTopic")
        inline API_RETURN_VALUE addComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.addComment")
        inline API_RETURN_VALUE delTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.delTopic")
        inline API_RETURN_VALUE editTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.editTopic")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.editComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("board.restoreComment")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.delComment")
        inline API_RETURN_VALUE openTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.openTopic")
        inline API_RETURN_VALUE closeTopic (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.closeTopic")
        inline API_RETURN_VALUE fixTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.fixTopic")
        inline API_RETURN_VALUE unfixTopic (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("board.unfixTopic")
    } board;

    class video_api {
        API_SUBCLASS_INIT(video_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("video.get")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("video.edit")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("video.add")
        inline API_RETURN_VALUE save (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("video.save")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("video.del")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("video.restore")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("video.search")
        inline API_RETURN_VALUE getUserVideos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.getUserVideos")
        inline API_RETURN_VALUE getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.getAlbums")
        inline API_RETURN_VALUE getAlbumById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.getAlbumById")
        inline API_RETURN_VALUE addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.addAlbum")
        inline API_RETURN_VALUE editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.editAlbum")
        inline API_RETURN_VALUE delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.delAlbum")
        inline API_RETURN_VALUE reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reorderAlbums")
        inline API_RETURN_VALUE reorderVideos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reorderVideos")
        inline API_RETURN_VALUE addToAlbum (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.addToAlbum")
        inline API_RETURN_VALUE removeFromAlbum (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("video.removeFromAlbum")
        inline API_RETURN_VALUE getAlbumsByVideo (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("video.getAlbumsByVideo")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.getComments")
        inline API_RETURN_VALUE createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.createComment")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.delComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("video.restoreComment")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.editComment")
        inline API_RETURN_VALUE getTags (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("video.getTags")
        inline API_RETURN_VALUE putTag (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("video.putTag")
        inline API_RETURN_VALUE removeTag (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.removeTag")
        inline API_RETURN_VALUE getNewTags (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.getNewTags")
        inline API_RETURN_VALUE report (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("video.report")
        inline API_RETURN_VALUE reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reportComment")
        inline API_RETURN_VALUE getCatalog (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("video.getCatalog")
        inline API_RETURN_VALUE getCatalogSection (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("video.getCatalogSection")
        inline API_RETURN_VALUE hideCatalogSection (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("video.hideCatalogSection")
    } video;

    class notes_api {
        API_SUBCLASS_INIT(notes_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("notes.get")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("notes.getById")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("notes.add")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("notes.edit")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("notes.del")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("notes.getComments")
        inline API_RETURN_VALUE createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("notes.createComment")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("notes.editComment")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("notes.delComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("notes.restoreComment")
    } notes;

    class places_api {
        API_SUBCLASS_INIT(places_api)
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("places.add")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("places.getById")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("places.search")
        inline API_RETURN_VALUE checkin (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("places.checkin")
        inline API_RETURN_VALUE getCheckins (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("places.getCheckins")
        inline API_RETURN_VALUE getTypes (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("places.getTypes")
    } places;

    class account_api {
        API_SUBCLASS_INIT(account_api)
        inline API_RETURN_VALUE getCounters (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("account.getCounters")
        inline API_RETURN_VALUE setNameInMenu (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.setNameInMenu")
        inline API_RETURN_VALUE setOnline (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.setOnline")
        inline API_RETURN_VALUE setOffline (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("account.setOffline")
        inline API_RETURN_VALUE lookupContacts (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.lookupContacts")
        inline API_RETURN_VALUE registerDevice (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.registerDevice")
        inline API_RETURN_VALUE unregisterDevice (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("account.unregisterDevice")
        inline API_RETURN_VALUE setSilenceMode (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.setSilenceMode")
        inline API_RETURN_VALUE getPushSettings (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.getPushSettings")
        inline API_RETURN_VALUE setPushSettings (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.setPushSettings")
        inline API_RETURN_VALUE getAppPermissions (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("account.getAppPermissions")
        inline API_RETURN_VALUE getActiveOffers (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.getActiveOffers")
        inline API_RETURN_VALUE banUser (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("account.banUser")
        inline API_RETURN_VALUE unbanUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.unbanUser")
        inline API_RETURN_VALUE getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.getBanned")
        inline API_RETURN_VALUE getInfo (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("account.getInfo")
        inline API_RETURN_VALUE setInfo (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("account.setInfo")
        inline API_RETURN_VALUE changePassword (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.changePassword")
        inline API_RETURN_VALUE getProfileInfo (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.getProfileInfo")
        inline API_RETURN_VALUE saveProfileInfo (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("account.saveProfileInfo")
    } account;

    class messages_api {
        API_SUBCLASS_INIT(messages_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("messages.get")
        inline API_RETURN_VALUE getDialogs (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.getDialogs")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("messages.getById")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)              	    API_SUBCLASS_METHOD_REQUEST("messages.search")
        inline API_RETURN_VALUE getHistory (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.getHistory")
        inline API_RETURN_VALUE getHistoryAttachments (API_METHOD_ARGS)	    API_SUBCLASS_METHOD_REQUEST("messages.getHistoryAttachments")
        inline API_RETURN_VALUE send (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("messages.send")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("messages.del")
        inline API_RETURN_VALUE delDialog (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.delDialog")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("messages.restore")
        inline API_RETURN_VALUE markAsRead (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.markAsRead")
        inline API_RETURN_VALUE markAsImportant (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("messages.markAsImportant")
        inline API_RETURN_VALUE getLongPollServer (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("messages.getLongPollServer")
        inline API_RETURN_VALUE getLongPollHistory (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("messages.getLongPollHistory")
        inline API_RETURN_VALUE getChat (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("messages.getChat")
        inline API_RETURN_VALUE createChat (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.createChat")
        inline API_RETURN_VALUE editChat (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.editChat")
        inline API_RETURN_VALUE getChatUsers (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.getChatUsers")
        inline API_RETURN_VALUE setActivity (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.setActivity")
        inline API_RETURN_VALUE searchDialogs (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.searchDialogs")
        inline API_RETURN_VALUE addChatUser (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("messages.addChatUser")
        inline API_RETURN_VALUE removeChatUser (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("messages.removeChatUser")
        inline API_RETURN_VALUE getLastActivity (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("messages.getLastActivity")
        inline API_RETURN_VALUE setChatPhoto (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.setChatPhoto")
        inline API_RETURN_VALUE delChatPhoto (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.delChatPhoto")
    } messages;

    class newsfeed_api {
        API_SUBCLASS_INIT (newsfeed_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("newsfeed.get")
        inline API_RETURN_VALUE getRecommended (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("newsfeed.getRecommended")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("newsfeed.getComments")
        inline API_RETURN_VALUE getMentions (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("newsfeed.getMentions")
        inline API_RETURN_VALUE getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getBanned")
        inline API_RETURN_VALUE addBan (API_METHOD_ARGS)              	    API_SUBCLASS_METHOD_REQUEST("newsfeed.addBan")
        inline API_RETURN_VALUE delBan (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("newsfeed.delBan")
        inline API_RETURN_VALUE ignoreItem (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("newsfeed.ignoreItem")
        inline API_RETURN_VALUE unignoreItem (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("newsfeed.unignoreItem")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("newsfeed.search")
        inline API_RETURN_VALUE getLists (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getLists")
        inline API_RETURN_VALUE saveList (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.saveList")
        inline API_RETURN_VALUE delList (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("newsfeed.delList")
        inline API_RETURN_VALUE unsubscribe (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("newsfeed.unsubscribe")
        inline API_RETURN_VALUE getSuggestedSources (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("newsfeed.getSuggestedSources")
    } newsfeed;

    class likes_api {
        API_SUBCLASS_INIT(likes_api)
        inline API_RETURN_VALUE getList (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("likes.getList")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("likes.add")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("likes.del")
        inline API_RETURN_VALUE isLiked (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("likes.isLiked")
    } likes;

    class polls_api {
        API_SUBCLASS_INIT(polls_api)
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("polls.getById")
        inline API_RETURN_VALUE addVote (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("polls.addVote")
        inline API_RETURN_VALUE delVote (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("polls.delVote")
        inline API_RETURN_VALUE getVoters (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("polls.getVoters")
        inline API_RETURN_VALUE create (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("polls.create")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("polls.edit")
    } polls;

    class docs_api {
        API_SUBCLASS_INIT(docs_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("docs.get")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("docs.getById")
        inline API_RETURN_VALUE getUploadServer (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("docs.getUploadServer")
        inline API_RETURN_VALUE getWallUploadServer (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("docs.getWallUploadServer")
        inline API_RETURN_VALUE save (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("docs.save")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("docs.del")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("docs.add")
        inline API_RETURN_VALUE getTypes (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("docs.getTypes")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)          		    API_SUBCLASS_METHOD_REQUEST("docs.search")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                	    API_SUBCLASS_METHOD_REQUEST("docs.edit")
    } docs;

    class fave_api {
        API_SUBCLASS_INIT(fave_api)
        inline API_RETURN_VALUE getUsers (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getUsers")
        inline API_RETURN_VALUE getPhotos (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getPhotos")
        inline API_RETURN_VALUE getPosts (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getPosts")
        inline API_RETURN_VALUE getVideos (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getVideos")
        inline API_RETURN_VALUE getLinks (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getLinks")
        inline API_RETURN_VALUE getMarketItems (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("fave.getMarketItems")
        inline API_RETURN_VALUE addUser (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("fave.addUser")
        inline API_RETURN_VALUE removeUser (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("fave.removeUser")
        inline API_RETURN_VALUE addGroup (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.addGroup")
        inline API_RETURN_VALUE removeGroup (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("fave.removeGroup")
        inline API_RETURN_VALUE addLink (API_METHOD_ARGS)         		    API_SUBCLASS_METHOD_REQUEST("fave.addLink")
        inline API_RETURN_VALUE removeLink (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("fave.removeLink")
    } fave;

    class notifications_api {
        API_SUBCLASS_INIT(notifications_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("notifications.get")
        inline API_RETURN_VALUE markAsViewed (API_METHOD_ARGS)              API_SUBCLASS_METHOD_REQUEST("notifications.markAsViewed")
    } notifications;

    class stats_api {
        API_SUBCLASS_INIT(stats_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("stats.get")
        inline API_RETURN_VALUE trackVisitor (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("stats.trackVisitor")
        inline API_RETURN_VALUE getPostReach (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("stats.getPostReach")
    } stats;

    class search_api {
        API_SUBCLASS_INIT(search_api)
        inline API_RETURN_VALUE getHints (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("search.getHints")
    } search;

    class apps_api {
        API_SUBCLASS_INIT(apps_api)
        inline API_RETURN_VALUE getCatalog (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("apps.getCatalog")
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("apps.get")
        inline API_RETURN_VALUE sendRequest (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("apps.sendRequest")
        inline API_RETURN_VALUE delAppRequests (API_METHOD_ARGS)            API_SUBCLASS_METHOD_REQUEST("apps.delAppRequests")
        inline API_RETURN_VALUE getFriendsList (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("apps.getFriendsList")
        inline API_RETURN_VALUE getLeaderboard (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("apps.getLeaderboard")
        inline API_RETURN_VALUE getScore (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("apps.getScore")
    } apps;

    class utils_api {
        API_SUBCLASS_INIT(utils_api)
        inline API_RETURN_VALUE checkLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("utils.checkLink")
        inline API_RETURN_VALUE resolveScreenName (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("utils.resolveScreenName")
        inline API_RETURN_VALUE getServerTime (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("utils.getServerTime")
    } utils;

    class database_api {
        API_SUBCLASS_INIT(database_api)
        inline API_RETURN_VALUE getCountries (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getCountries")
        inline API_RETURN_VALUE getRegions (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("database.getRegions")
        inline API_RETURN_VALUE getStreetsById (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("database.getStreetsById")
        inline API_RETURN_VALUE getCountriesById (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("database.getCountriesById")
        inline API_RETURN_VALUE getCities (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getCities")
        inline API_RETURN_VALUE getCitiesById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getCitiesById")
        inline API_RETURN_VALUE getUniversities (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("database.getUniversities")
        inline API_RETURN_VALUE getSchools (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("database.getSchools")
        inline API_RETURN_VALUE getSchoolClasses (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("database.getSchoolClasses")
        inline API_RETURN_VALUE getFaculties (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getFaculties")
        inline API_RETURN_VALUE getChairs (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getChairs")
    } database;

    class gifts_api {
        API_SUBCLASS_INIT(gifts_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("gifts.get")
    } gifts;

    class market_api {
        API_SUBCLASS_INIT(market_api)
        inline API_RETURN_VALUE get (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("market.get")
        inline API_RETURN_VALUE getById (API_METHOD_ARGS)                   API_SUBCLASS_METHOD_REQUEST("market.getById")
        inline API_RETURN_VALUE search (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("market.search")
        inline API_RETURN_VALUE getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.getAlbums")
        inline API_RETURN_VALUE getAlbumById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.getAlbumById")
        inline API_RETURN_VALUE createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.createComment")
        inline API_RETURN_VALUE getComments (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("market.getComments")
        inline API_RETURN_VALUE delComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("market.delComment")
        inline API_RETURN_VALUE restoreComment (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("market.restoreComment")
        inline API_RETURN_VALUE editComment (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("market.editComment")
        inline API_RETURN_VALUE reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reportComment")
        inline API_RETURN_VALUE getCategories (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.getCategories")
        inline API_RETURN_VALUE report (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("market.report")
        inline API_RETURN_VALUE add (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("market.add")
        inline API_RETURN_VALUE edit (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("market.edit")
        inline API_RETURN_VALUE del (API_METHOD_ARGS)             		    API_SUBCLASS_METHOD_REQUEST("market.del")
        inline API_RETURN_VALUE restore (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("market.restore")
        inline API_RETURN_VALUE reorderItems (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reorderItems")
        inline API_RETURN_VALUE reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reorderAlbums")
        inline API_RETURN_VALUE addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.addAlbum")
        inline API_RETURN_VALUE editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.editAlbum")
        inline API_RETURN_VALUE delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.delAlbum")
        inline API_RETURN_VALUE removeFromAlbum (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("market.removeFromAlbum")
        inline API_RETURN_VALUE addToAlbum (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("market.addToAlbum")
    } market;

private:
    /* CURL Write Function to read data from API */
    static size_t CurlWriteDataCallback(void* contents, size_t size, size_t nmemb, void* useptr);

    void ReadDataToJSON();

    void CustomRequest(const string& url, const string& method, const Args& arguments);

    void HandleError(const VKValue& json);

    const string GenerateURL(const string& url, const string& method, const Args& arguments);

    string   app_id;
    string   app_secret;

    VKValue        json;
    CURL*          curl_handle;
    CURLcode       curl_errno;
    VKResultCode_t vk_errno;

    string   def_access_token;
    string   def_api_version;
    string   def_lang;

    string   buffer;

    uint8_t      max_requests_per_second;
    uint8_t      request_counter;
    milliseconds last_time;
};

}

#endif // VKAPI_VKAPI_H
