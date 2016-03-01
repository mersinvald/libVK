#ifndef VKAPI_H
#define VKAPI_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <map>
#include <curl/curl.h>

#include "types.hpp"
#include "vkjson.hpp"

namespace vk {

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

#define VKAPI_URL       "https://api.vk.com/method/"
#define VKAPI_AUTH_URL  "https://oauth.vk.com/"

#define API_SUBCLASS_INIT(name) \
    private: VKAPI* this_ptr; \
    public:  name (VKAPI* ptr) : this_ptr(ptr) {}

#define API_METHOD_ARGS                       Args& args
#define API_SUBCLASS_METHOD_REQUEST(method) { return this_ptr->Request((method), args); }
#define API_METHOD_REQUEST(method)          { return           Request((method), args); }

class VKAPI {
public:
    VKAPI();
    VKAPI(const string app_id, const string app_secret);

    /* Base functionality */

    VKResultCode_t Authorize(const string login, const string passwd, string* access_token = NULL);
    VKResultCode_t Request(const string method, Args& arguments);

    /* Setters */

    void SetAppID            (const string app_id);
    void SetAppSecret        (const string app_secret);
    void SetefaultAccessToken(const string token);
    void SetDefaultAPIVersion(const string version);

    /* Getters */

    CURLcode       getCurlError()   const;
    VKResultCode_t getVKError()     const;
    const VKValue& getJSON()        const;
    string         getAccessToken() const;

    /* API methods */
    inline VKResultCode_t execute (API_METHOD_ARGS)                       	API_METHOD_REQUEST("execute")

    class users_api {
        API_SUBCLASS_INIT(users_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("users.get")
        inline VKResultCode_t search (API_METHOD_ARGS)                  	API_SUBCLASS_METHOD_REQUEST("users.search")
        inline VKResultCode_t isAppUser (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("users.isAppUser")
        inline VKResultCode_t getSubscriptions (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("users.getSubscriptions")
        inline VKResultCode_t getFollowers (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("users.getFollowers")
        inline VKResultCode_t report (API_METHOD_ARGS)                  	API_SUBCLASS_METHOD_REQUEST("users.report")
        inline VKResultCode_t getNearby (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("users.getNearby")
    } users;

    class auth_api {
        API_SUBCLASS_INIT(auth_api)
        inline VKResultCode_t checkPhone (API_METHOD_ARGS)          		API_SUBCLASS_METHOD_REQUEST("auth.checkPhone")
        inline VKResultCode_t signup (API_METHOD_ARGS)              		API_SUBCLASS_METHOD_REQUEST("auth.signup")
        inline VKResultCode_t confirm (API_METHOD_ARGS)             		API_SUBCLASS_METHOD_REQUEST("auth.confirm")
        inline VKResultCode_t restore (API_METHOD_ARGS)             		API_SUBCLASS_METHOD_REQUEST("auth.restore")
    } auth;

    class wall_api {
        API_SUBCLASS_INIT(wall_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                 		API_SUBCLASS_METHOD_REQUEST("wall.get")
        inline VKResultCode_t search (API_METHOD_ARGS)          			API_SUBCLASS_METHOD_REQUEST("wall.search")
        inline VKResultCode_t getById (API_METHOD_ARGS)            			API_SUBCLASS_METHOD_REQUEST("wall.getById")
        inline VKResultCode_t post (API_METHOD_ARGS)                		API_SUBCLASS_METHOD_REQUEST("wall.post")
        inline VKResultCode_t repost (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("wall.repost")
        inline VKResultCode_t getReposts (API_METHOD_ARGS)              	API_SUBCLASS_METHOD_REQUEST("wall.getReposts")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("wall.edit")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("wall.del")
        inline VKResultCode_t restore (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("wall.restore")
        inline VKResultCode_t pin (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("wall.pin")
        inline VKResultCode_t unpin (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("wall.unpin")
        inline VKResultCode_t getComments (API_METHOD_ARGS)                 API_SUBCLASS_METHOD_REQUEST("wall.getComments")
        inline VKResultCode_t addComment (API_METHOD_ARGS)                  API_SUBCLASS_METHOD_REQUEST("wall.addComment")
        inline VKResultCode_t editComment (API_METHOD_ARGS)                 API_SUBCLASS_METHOD_REQUEST("wall.editComment")
        inline VKResultCode_t delComment (API_METHOD_ARGS)                  API_SUBCLASS_METHOD_REQUEST("wall.delComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)              API_SUBCLASS_METHOD_REQUEST("wall.restoreComment")
        inline VKResultCode_t reportPost (API_METHOD_ARGS)                  API_SUBCLASS_METHOD_REQUEST("wall.reportPost")
        inline VKResultCode_t reportComment (API_METHOD_ARGS)               API_SUBCLASS_METHOD_REQUEST("wall.reportComment")
    } wall;

    class photos_api {
        API_SUBCLASS_INIT(photos_api)
        inline VKResultCode_t createAlbum (API_METHOD_ARGS)         		API_SUBCLASS_METHOD_REQUEST("photos.createAlbum")
        inline VKResultCode_t editAlbum (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("photos.editAlbum")
        inline VKResultCode_t getAlbums (API_METHOD_ARGS)           		API_SUBCLASS_METHOD_REQUEST("photos.getAlbums")
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("photos.get")
        inline VKResultCode_t getAlbumsCount (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.getAlbumsCount")
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("photos.getById")
        inline VKResultCode_t getUploadServer (API_METHOD_ARGS)				API_SUBCLASS_METHOD_REQUEST("photos.getUploadServer")
        inline VKResultCode_t getOwnerPhotoUploadServer (API_METHOD_ARGS)	API_SUBCLASS_METHOD_REQUEST("photos.getOwnerPhotoUploadServer")
        inline VKResultCode_t getChatUploadServer (API_METHOD_ARGS)			API_SUBCLASS_METHOD_REQUEST("photos.getChatUploadServer")
        inline VKResultCode_t getMarketUploadServer (API_METHOD_ARGS)		API_SUBCLASS_METHOD_REQUEST("photos.getMarketUploadServer")
        inline VKResultCode_t getMarketAlbumUploadServer (API_METHOD_ARGS)  API_SUBCLASS_METHOD_REQUEST("photos.getMarketAlbumUploadServer")
        inline VKResultCode_t saveMarketPhoto (API_METHOD_ARGS)             API_SUBCLASS_METHOD_REQUEST("photos.saveMarketPhoto")
        inline VKResultCode_t saveMarketAlbumPhoto (API_METHOD_ARGS)    	API_SUBCLASS_METHOD_REQUEST("photos.saveMarketAlbumPhoto")
        inline VKResultCode_t saveOwnerPhoto (API_METHOD_ARGS)          	API_SUBCLASS_METHOD_REQUEST("photos.saveOwnerPhoto")
        inline VKResultCode_t saveWallPhoto (API_METHOD_ARGS)               API_SUBCLASS_METHOD_REQUEST("photos.saveWallPhoto")
        inline VKResultCode_t getWallUploadServer (API_METHOD_ARGS)         API_SUBCLASS_METHOD_REQUEST("photos.getWallUploadServer")
        inline VKResultCode_t getMessagesUploadServer (API_METHOD_ARGS) 	API_SUBCLASS_METHOD_REQUEST("photos.getMessagesUploadServer")
        inline VKResultCode_t saveMessagesPhoto (API_METHOD_ARGS)           API_SUBCLASS_METHOD_REQUEST("photos.saveMessagesPhoto")
        inline VKResultCode_t report (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.report")
        inline VKResultCode_t reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reportComment")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.search")
        inline VKResultCode_t save (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("photos.save")
        inline VKResultCode_t copy (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("photos.copy")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("photos.edit")
        inline VKResultCode_t move (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("photos.move")
        inline VKResultCode_t makeCover (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.makeCover")
        inline VKResultCode_t reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reorderAlbums")
        inline VKResultCode_t reorderPhotos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.reorderPhotos")
        inline VKResultCode_t getAll (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.getAll")
        inline VKResultCode_t getUserPhotos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.getUserPhotos")
        inline VKResultCode_t delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.delAlbum")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("photos.del")
        inline VKResultCode_t restore (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("photos.restore")
        inline VKResultCode_t confirmTag (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.confirmTag")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.getComments")
        inline VKResultCode_t getAllComments (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.getAllComments")
        inline VKResultCode_t createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.createComment")
        inline VKResultCode_t delComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.delComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("photos.restoreComment")
        inline VKResultCode_t editComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.editComment")
        inline VKResultCode_t getTags (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("photos.getTags")
        inline VKResultCode_t putTag (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("photos.putTag")
        inline VKResultCode_t removeTag (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.removeTag")
        inline VKResultCode_t getNewTags (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("photos.getNewTags")
    } photos;

    class friends_api {
        API_SUBCLASS_INIT(friends_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("friends.get")
        inline VKResultCode_t getOnline (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getOnline")
        inline VKResultCode_t getMutual (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getMutual")
        inline VKResultCode_t getRecent (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getRecent")
        inline VKResultCode_t getRequests (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getRequests")
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("friends.add")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("friends.edit")
        inline VKResultCode_t del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("friends.del")
        inline VKResultCode_t getLists (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getLists")
        inline VKResultCode_t addList (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("friends.addList")
        inline VKResultCode_t editList (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.editList")
        inline VKResultCode_t delList (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("friends.delList")
        inline VKResultCode_t getAppUsers (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getAppUsers")
        inline VKResultCode_t getByPhones (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.getByPhones")
        inline VKResultCode_t delAllRequests (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("friends.delAllRequests")
        inline VKResultCode_t getSuggestions (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("friends.getSuggestions")
        inline VKResultCode_t areFriends (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("friends.areFriends")
        inline VKResultCode_t getAvailableForCall (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("friends.getAvailableForCall")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("friends.search")
    } friends;

    class widgets_api {
        API_SUBCLASS_INIT(widgets_api)
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("widgets.getComments")
        inline VKResultCode_t getPages (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("widgets.getPages")
    } widgets;

    class storage_api {
        API_SUBCLASS_INIT(storage_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("storage.get")
        inline VKResultCode_t set (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("storage.set")
        inline VKResultCode_t getKeys (API_METHOD_ARGS)         		    API_SUBCLASS_METHOD_REQUEST("storage.getKeys")
    } storage;

    class status_api {
        API_SUBCLASS_INIT(status_api)
        inline VKResultCode_t get (API_METHOD_ARGS)         			    API_SUBCLASS_METHOD_REQUEST("status.get")
        inline VKResultCode_t set (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("status.set")
    } status;

    class audio_api {
        API_SUBCLASS_INIT(audio_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("audio.get")
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("audio.getById")
        inline VKResultCode_t getLyrics (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.getLyrics")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("audio.search")
        inline VKResultCode_t getUploadServer (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("audio.getUploadServer")
        inline VKResultCode_t save (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("audio.save")
        inline VKResultCode_t add (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("audio.add")
        inline VKResultCode_t del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("audio.del")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("audio.edit")
        inline VKResultCode_t reorder (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("audio.reorder")
        inline VKResultCode_t restore (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("audio.restore")
        inline VKResultCode_t getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.getAlbums")
        inline VKResultCode_t addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.addAlbum")
        inline VKResultCode_t editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.editAlbum")
        inline VKResultCode_t delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.delAlbum")
        inline VKResultCode_t moveToAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.moveToAlbum")
        inline VKResultCode_t setBroadcast (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("audio.setBroadcast")
        inline VKResultCode_t getBroadcastList (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("audio.getBroadcastList")
        inline VKResultCode_t getRecommendations (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("audio.getRecommendations")
        inline VKResultCode_t getPopular (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("audio.getPopular")
        inline VKResultCode_t getCount (API_METHOD_ARGS)                    API_SUBCLASS_METHOD_REQUEST("audio.getCount")
    } audio;

    class pages_api {
        API_SUBCLASS_INIT(pages_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("pages.get")
        inline VKResultCode_t save (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("pages.save")
        inline VKResultCode_t saveAccess (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.saveAccess")
        inline VKResultCode_t getHistory (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.getHistory")
        inline VKResultCode_t getTitles (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.getTitles")
        inline VKResultCode_t getVersion (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.getVersion")
        inline VKResultCode_t parseWiki (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.parseWiki")
        inline VKResultCode_t clearCache (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("pages.clearCache")
    } pages;

    class groups_api {
        API_SUBCLASS_INIT(groups_api)
        inline VKResultCode_t isMember (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.isMember")
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("groups.getById")
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("groups.get")
        inline VKResultCode_t getMembers (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getMembers")
        inline VKResultCode_t join (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("groups.join")
        inline VKResultCode_t leave (API_METHOD_ARGS)                       API_SUBCLASS_METHOD_REQUEST("groups.leave")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("groups.search")
        inline VKResultCode_t getCatalog (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getCatalog")
        inline VKResultCode_t getCatalogInfo (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("groups.getCatalogInfo")
        inline VKResultCode_t getInvites (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getInvites")
        inline VKResultCode_t getInvitedUsers (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("groups.getInvitedUsers")
        inline VKResultCode_t banUser (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("groups.banUser")
        inline VKResultCode_t unbanUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.unbanUser")
        inline VKResultCode_t getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getBanned")
        inline VKResultCode_t create (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("groups.create")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("groups.edit")
        inline VKResultCode_t editPlace (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.editPlace")
        inline VKResultCode_t getSettings (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getSettings")
        inline VKResultCode_t getRequests (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.getRequests")
        inline VKResultCode_t editManager (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.editManager")
        inline VKResultCode_t invite (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("groups.invite")
        inline VKResultCode_t addLink (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("groups.addLink")
        inline VKResultCode_t delLink (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("groups.delLink")
        inline VKResultCode_t editLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.editLink")
        inline VKResultCode_t reorderLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.reorderLink")
        inline VKResultCode_t removeUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("groups.removeUser")
        inline VKResultCode_t approveRequest (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("groups.approveRequest")
    } groups;

    class board_api {
        API_SUBCLASS_INIT(board_api)
        inline VKResultCode_t getTopics (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.getTopics")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.getComments")
        inline VKResultCode_t addTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.addTopic")
        inline VKResultCode_t addComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.addComment")
        inline VKResultCode_t delTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.delTopic")
        inline VKResultCode_t editTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.editTopic")
        inline VKResultCode_t editComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.editComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("board.restoreComment")
        inline VKResultCode_t delComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.delComment")
        inline VKResultCode_t openTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.openTopic")
        inline VKResultCode_t closeTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.closeTopic")
        inline VKResultCode_t fixTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.fixTopic")
        inline VKResultCode_t unfixTopic (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("board.unfixTopic")
    } board;

    class video_api {
        API_SUBCLASS_INIT(video_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("video.get")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("video.edit")
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("video.add")
        inline VKResultCode_t save (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("video.save")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("video.del")
        inline VKResultCode_t restore (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("video.restore")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("video.search")
        inline VKResultCode_t getUserVideos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.getUserVideos")
        inline VKResultCode_t getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.getAlbums")
        inline VKResultCode_t getAlbumById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.getAlbumById")
        inline VKResultCode_t addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.addAlbum")
        inline VKResultCode_t editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.editAlbum")
        inline VKResultCode_t delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.delAlbum")
        inline VKResultCode_t reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reorderAlbums")
        inline VKResultCode_t reorderVideos (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reorderVideos")
        inline VKResultCode_t addToAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.addToAlbum")
        inline VKResultCode_t removeFromAlbum (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.removeFromAlbum")
        inline VKResultCode_t getAlbumsByVideo (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("video.getAlbumsByVideo")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.getComments")
        inline VKResultCode_t createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.createComment")
        inline VKResultCode_t delComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.delComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.restoreComment")
        inline VKResultCode_t editComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.editComment")
        inline VKResultCode_t getTags (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("video.getTags")
        inline VKResultCode_t putTag (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("video.putTag")
        inline VKResultCode_t removeTag (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.removeTag")
        inline VKResultCode_t getNewTags (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.getNewTags")
        inline VKResultCode_t report (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("video.report")
        inline VKResultCode_t reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("video.reportComment")
        inline VKResultCode_t getCatalog (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("video.getCatalog")
        inline VKResultCode_t getCatalogSection (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("video.getCatalogSection")
        inline VKResultCode_t hideCatalogSection (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("video.hideCatalogSection")
    } video;

    class notes_api {
        API_SUBCLASS_INIT(notes_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("notes.get")
        inline VKResultCode_t getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("notes.getById")
        inline VKResultCode_t add (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("notes.add")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("notes.edit")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("notes.del")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("notes.getComments")
        inline VKResultCode_t createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("notes.createComment")
        inline VKResultCode_t editComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("notes.editComment")
        inline VKResultCode_t delComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("notes.delComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("notes.restoreComment")
    } notes;

    class places_api {
        API_SUBCLASS_INIT(places_api)
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("places.add")
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("places.getById")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("places.search")
        inline VKResultCode_t checkin (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("places.checkin")
        inline VKResultCode_t getCheckins (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("places.getCheckins")
        inline VKResultCode_t getTypes (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("places.getTypes")
    } places;

    class account_api {
        API_SUBCLASS_INIT(account_api)
        inline VKResultCode_t getCounters (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.getCounters")
        inline VKResultCode_t setNameInMenu (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.setNameInMenu")
        inline VKResultCode_t setOnline (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.setOnline")
        inline VKResultCode_t setOffline (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.setOffline")
        inline VKResultCode_t lookupContacts (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.lookupContacts")
        inline VKResultCode_t registerDevice (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.registerDevice")
        inline VKResultCode_t unregisterDevice (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("account.unregisterDevice")
        inline VKResultCode_t setSilenceMode (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.setSilenceMode")
        inline VKResultCode_t getPushSettings (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.getPushSettings")
        inline VKResultCode_t setPushSettings (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.setPushSettings")
        inline VKResultCode_t getAppPermissions (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("account.getAppPermissions")
        inline VKResultCode_t getActiveOffers (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.getActiveOffers")
        inline VKResultCode_t banUser (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("account.banUser")
        inline VKResultCode_t unbanUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.unbanUser")
        inline VKResultCode_t getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("account.getBanned")
        inline VKResultCode_t getInfo (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("account.getInfo")
        inline VKResultCode_t setInfo (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("account.setInfo")
        inline VKResultCode_t changePassword (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.changePassword")
        inline VKResultCode_t getProfileInfo (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.getProfileInfo")
        inline VKResultCode_t saveProfileInfo (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("account.saveProfileInfo")
    } account;

    class messages_api {
        API_SUBCLASS_INIT(messages_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("messages.get")
        inline VKResultCode_t getDialogs (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.getDialogs")
        inline VKResultCode_t getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("messages.getById")
        inline VKResultCode_t search (API_METHOD_ARGS)              	    API_SUBCLASS_METHOD_REQUEST("messages.search")
        inline VKResultCode_t getHistory (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.getHistory")
        inline VKResultCode_t getHistoryAttachments (API_METHOD_ARGS)	    API_SUBCLASS_METHOD_REQUEST("messages.getHistoryAttachments")
        inline VKResultCode_t send (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("messages.send")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("messages.del")
        inline VKResultCode_t delDialog (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.delDialog")
        inline VKResultCode_t restore (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("messages.restore")
        inline VKResultCode_t markAsRead (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.markAsRead")
        inline VKResultCode_t markAsImportant (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.markAsImportant")
        inline VKResultCode_t getLongPollServer (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("messages.getLongPollServer")
        inline VKResultCode_t getLongPollHistory (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("messages.getLongPollHistory")
        inline VKResultCode_t getChat (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("messages.getChat")
        inline VKResultCode_t createChat (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.createChat")
        inline VKResultCode_t editChat (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.editChat")
        inline VKResultCode_t getChatUsers (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.getChatUsers")
        inline VKResultCode_t setActivity (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.setActivity")
        inline VKResultCode_t searchDialogs (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.searchDialogs")
        inline VKResultCode_t addChatUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("messages.addChatUser")
        inline VKResultCode_t removeChatUser (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.removeChatUser")
        inline VKResultCode_t getLastActivity (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.getLastActivity")
        inline VKResultCode_t setChatPhoto (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.setChatPhoto")
        inline VKResultCode_t delChatPhoto (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("messages.delChatPhoto")
    } messages;

    class newsfeed_api {
        API_SUBCLASS_INIT (newsfeed_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("newsfeed.get")
        inline VKResultCode_t getRecommended (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("newsfeed.getRecommended")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getComments")
        inline VKResultCode_t getMentions (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getMentions")
        inline VKResultCode_t getBanned (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getBanned")
        inline VKResultCode_t addBan (API_METHOD_ARGS)              	    API_SUBCLASS_METHOD_REQUEST("newsfeed.addBan")
        inline VKResultCode_t delBan (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("newsfeed.delBan")
        inline VKResultCode_t ignoreItem (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.ignoreItem")
        inline VKResultCode_t unignoreItem (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("newsfeed.unignoreItem")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("newsfeed.search")
        inline VKResultCode_t getLists (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.getLists")
        inline VKResultCode_t saveList (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.saveList")
        inline VKResultCode_t delList (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("newsfeed.delList")
        inline VKResultCode_t unsubscribe (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("newsfeed.unsubscribe")
        inline VKResultCode_t getSuggestedSources (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("newsfeed.getSuggestedSources")
    } newsfeed;

    class likes_api {
        API_SUBCLASS_INIT(likes_api)
        inline VKResultCode_t getList (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("likes.getList")
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("likes.add")
        inline VKResultCode_t del (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("likes.del")
        inline VKResultCode_t isLiked (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("likes.isLiked")
    } likes;

    class polls_api {
        API_SUBCLASS_INIT(polls_api)
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("polls.getById")
        inline VKResultCode_t addVote (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("polls.addVote")
        inline VKResultCode_t delVote (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("polls.delVote")
        inline VKResultCode_t getVoters (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("polls.getVoters")
        inline VKResultCode_t create (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("polls.create")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("polls.edit")
    } polls;

    class docs_api {
        API_SUBCLASS_INIT(docs_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("docs.get")
        inline VKResultCode_t getById (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("docs.getById")
        inline VKResultCode_t getUploadServer (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("docs.getUploadServer")
        inline VKResultCode_t getWallUploadServer (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("docs.getWallUploadServer")
        inline VKResultCode_t save (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("docs.save")
        inline VKResultCode_t del (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("docs.del")
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("docs.add")
        inline VKResultCode_t getTypes (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("docs.getTypes")
        inline VKResultCode_t search (API_METHOD_ARGS)          		    API_SUBCLASS_METHOD_REQUEST("docs.search")
        inline VKResultCode_t edit (API_METHOD_ARGS)                	    API_SUBCLASS_METHOD_REQUEST("docs.edit")
    } docs;

    class fave_api {
        API_SUBCLASS_INIT(fave_api)
        inline VKResultCode_t getUsers (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getUsers")
        inline VKResultCode_t getPhotos (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getPhotos")
        inline VKResultCode_t getPosts (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getPosts")
        inline VKResultCode_t getVideos (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getVideos")
        inline VKResultCode_t getLinks (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.getLinks")
        inline VKResultCode_t getMarketItems (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("fave.getMarketItems")
        inline VKResultCode_t addUser (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("fave.addUser")
        inline VKResultCode_t removeUser (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.removeUser")
        inline VKResultCode_t addGroup (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.addGroup")
        inline VKResultCode_t removeGroup (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.removeGroup")
        inline VKResultCode_t addLink (API_METHOD_ARGS)         		    API_SUBCLASS_METHOD_REQUEST("fave.addLink")
        inline VKResultCode_t removeLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("fave.removeLink")
    } fave;

    class notifications_api {
        API_SUBCLASS_INIT(notifications_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("notifications.get")
        inline VKResultCode_t markAsViewed (API_METHOD_ARGS)                API_SUBCLASS_METHOD_REQUEST("notifications.markAsViewed")
    } notifications;

    class stats_api {
        API_SUBCLASS_INIT(stats_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("stats.get")
        inline VKResultCode_t trackVisitor (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("stats.trackVisitor")
        inline VKResultCode_t getPostReach (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("stats.getPostReach")
    } stats;

    class search_api {
        API_SUBCLASS_INIT(search_api)
        inline VKResultCode_t getHints (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("search.getHints")
    } search;

    class apps_api {
        API_SUBCLASS_INIT(apps_api)
        inline VKResultCode_t getCatalog (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("apps.getCatalog")
        inline VKResultCode_t get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("apps.get")
        inline VKResultCode_t sendRequest (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("apps.sendRequest")
        inline VKResultCode_t delAppRequests (API_METHOD_ARGS)              API_SUBCLASS_METHOD_REQUEST("apps.delAppRequests")
        inline VKResultCode_t getFriendsList (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("apps.getFriendsList")
        inline VKResultCode_t getLeaderboard (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("apps.getLeaderboard")
        inline VKResultCode_t getScore (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("apps.getScore")
    } apps;

    class utils_api {
        API_SUBCLASS_INIT(utils_api)
        inline VKResultCode_t checkLink (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("utils.checkLink")
        inline VKResultCode_t resolveScreenName (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("utils.resolveScreenName")
        inline VKResultCode_t getServerTime (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("utils.getServerTime")
    } utils;

    class database_api {
        API_SUBCLASS_INIT(database_api)
        inline VKResultCode_t getCountries (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getCountries")
        inline VKResultCode_t getRegions (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getRegions")
        inline VKResultCode_t getStreetsById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getStreetsById")
        inline VKResultCode_t getCountriesById (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("database.getCountriesById")
        inline VKResultCode_t getCities (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getCities")
        inline VKResultCode_t getCitiesById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getCitiesById")
        inline VKResultCode_t getUniversities (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getUniversities")
        inline VKResultCode_t getSchools (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getSchools")
        inline VKResultCode_t getSchoolClasses (API_METHOD_ARGS)		    API_SUBCLASS_METHOD_REQUEST("database.getSchoolClasses")
        inline VKResultCode_t getFaculties (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("database.getFaculties")
        inline VKResultCode_t getChairs (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("database.getChairs")
    } database;

    class gifts_api {
        API_SUBCLASS_INIT(gifts_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                 	    API_SUBCLASS_METHOD_REQUEST("gifts.get")
    } gifts;

    class market_api {
        API_SUBCLASS_INIT(market_api)
        inline VKResultCode_t get (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("market.get")
        inline VKResultCode_t getById (API_METHOD_ARGS)                     API_SUBCLASS_METHOD_REQUEST("market.getById")
        inline VKResultCode_t search (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("market.search")
        inline VKResultCode_t getAlbums (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.getAlbums")
        inline VKResultCode_t getAlbumById (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.getAlbumById")
        inline VKResultCode_t createComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.createComment")
        inline VKResultCode_t getComments (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.getComments")
        inline VKResultCode_t delComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.delComment")
        inline VKResultCode_t restoreComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.restoreComment")
        inline VKResultCode_t editComment (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.editComment")
        inline VKResultCode_t reportComment (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reportComment")
        inline VKResultCode_t getCategories (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.getCategories")
        inline VKResultCode_t report (API_METHOD_ARGS)                      API_SUBCLASS_METHOD_REQUEST("market.report")
        inline VKResultCode_t add (API_METHOD_ARGS)                         API_SUBCLASS_METHOD_REQUEST("market.add")
        inline VKResultCode_t edit (API_METHOD_ARGS)                        API_SUBCLASS_METHOD_REQUEST("market.edit")
        inline VKResultCode_t del (API_METHOD_ARGS)             		    API_SUBCLASS_METHOD_REQUEST("market.del")
        inline VKResultCode_t restore (API_METHOD_ARGS)             	    API_SUBCLASS_METHOD_REQUEST("market.restore")
        inline VKResultCode_t reorderItems (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reorderItems")
        inline VKResultCode_t reorderAlbums (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.reorderAlbums")
        inline VKResultCode_t addAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.addAlbum")
        inline VKResultCode_t editAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.editAlbum")
        inline VKResultCode_t delAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.delAlbum")
        inline VKResultCode_t removeFromAlbum (API_METHOD_ARGS)			    API_SUBCLASS_METHOD_REQUEST("market.removeFromAlbum")
        inline VKResultCode_t addToAlbum (API_METHOD_ARGS)				    API_SUBCLASS_METHOD_REQUEST("market.addToAlbum")
    } market;

private:
    /* CURL Write Function to read data from API */
    static size_t CurlWriteDataCallback(void* contents, size_t size, size_t nmemb, void* useptr);

    VKResultCode_t ReadDataToJSON();

    VKResultCode_t CustomRequest(const string url, const string method, const Args& arguments);

    VKResultCode_t HandleError(const VKValue json);

    const string GenerateURL(const string url, const string method, const Args& arguments);

    string   app_id;
    string   app_secret;

    VKValue        json;
    CURL*          curl_handle;
    CURLcode       curl_errno;
    VKResultCode_t vk_errno;

    string   def_access_token;
    string   def_api_version;

    string   buffer;
};

}

#endif // VKAPI_H
