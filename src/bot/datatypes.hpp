//
// Created by X-ray on 8/17/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_DATATYPES_HPP
#define DISCORD_CPP_BOT_DATATYPES_HPP
#include <string>
#include <vector>
#include <rapidjson/document.h>

namespace bot {
    namespace datatypes {
        typedef rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>> rapidjson_object_t;
        typedef rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>> const_rapidjson_object_t;

        inline std::string GetString(rapidjson_object_t json, const char* key) {
            if (json.HasMember(key)) {
                if (json[key].IsString()) {
                    return json[key].GetString();
                }
            }
            return std::string("");
        }

        inline uint32_t GetInt(rapidjson_object_t json, const char* key) {
            if (json.HasMember(key)) {
                if (json[key].IsInt()) {
                    return json[key].GetInt();
                }
            }
            return NULL;
        }

        inline uint64_t GetInt64(rapidjson_object_t json, const char* key) {
            if (json.HasMember(key)) {
                if (json[key].IsInt64()) {
                    return json[key].GetInt64();
                }
            }
            return NULL;
        }

        inline bool GetBool(rapidjson_object_t json, const char* key) {
            if (json.HasMember(key)) {
                if (json[key].IsBool()) {
                    return json[key].GetBool();
                }
            }
            return false;
        }

        inline rapidjson_object_t GetObject(rapidjson_object_t json, const char* key) {
            if (json.HasMember(key)) {
                if (json[key].IsObject()) {
                    return json[key].GetObject();
                }
            }
            return json;
        }

        namespace user {
            struct user_t {
                user_t() = default;

                explicit user_t(rapidjson_object_t json) {
                    this->id = GetString(json, "id");
                    this->username = GetString(json, "username");
                    this->discriminator = GetString(json, "discriminator");
                    this->avatar = GetString(json, "avatar");
                    this->bot = GetBool(json, "bot");
                    this->system = GetBool(json, "bot");
                    this->mfa_enabled = GetBool(json, "mfa_enabled");
                    this->locale = GetString(json, "locale");
                    this->verified = GetBool(json, "verified");
                    this->email = GetString(json, "email");
                    this->flags = GetInt(json, "flags");
                    this->premium = GetBool(json, "premium");
                    this->public_flags = GetInt(json, "public_flags");
                }

                std::string id;
                std::string username;
                std::string discriminator;
                std::string avatar;
                bool bot = false;
                bool system = false;
                bool mfa_enabled = false;
                std::string locale;
                bool verified = false;
                std::string email;
                uint32_t flags = NULL;
                bool premium = false;
                uint32_t public_flags = NULL;
            };
        }
        namespace channel {
            namespace overwrite {
                struct overwrite_t {
                    explicit overwrite_t(rapidjson_object_t json) {
                        this->id = GetString(json, "id");
                        this->type = GetInt(json, "type");
                        this->allow = GetBool(json, "allow");
                        this->deny = GetBool(json, "deny");
                    }

                    std::string id;
                    uint32_t type;
                    std::string allow;
                    std::string deny;
                };
            }

            namespace thread {
                namespace metadata {
                    struct metadata_t {
                        explicit metadata_t(rapidjson_object_t json) {
                            this->archived = GetBool(json, "archived");
                            this->auto_archive_duration = GetInt(json, "auto_archive_duration");
                            this->archive_timestamp = GetString(json, "archive_timestamp");
                            this->locked = GetBool(json, " bool");
                        }

                        bool archived;
                        uint32_t auto_archive_duration;
                        std::string archive_timestamp;
                        bool locked;
                    };
                }
                namespace member {
                    struct member_t {
                        explicit member_t(rapidjson_object_t json) {
                            this->id = GetString(json, "id");
                            this->user_id = GetString(json, "user_id");
                            this->join_timestamp = GetString(json, "join_timestamp");
                            this->flags = GetInt(json, "flags");
                        }

                        std::string id;
                        std::string user_id;
                        std::string join_timestamp;
                        uint32_t flags;
                    };
                }
            }

            enum class channeltype : uint32_t {
                guild_text,
                dm,
                guild_voice,
                group_dm,
                guild_category,
                guild_news,
                guild_store,
                guild_news_thread = 10,
                guild_public_thread,
                guild_private_thread,
                guild_stage_voice,
                };

            struct channel_t {
                explicit channel_t(rapidjson_object_t json) : metadata(GetObject(json, "thread_metadata")), member(GetObject(json, "member")) {
                    this->id = GetString(json, "id");
                    this->type = static_cast<channeltype>(GetInt(json, "type"));
                    this->guild_id = GetString(json, "guild_id");
                    this->position = GetInt(json, "position");
                    if (json.HasMember("permission_overwrites")) {
                        for (rapidjson::SizeType i = 0; i < json["permission_overwrites"].Size(); i++) {
                            this->permission_overwrites.emplace_back(overwrite::overwrite_t(json["permission_overwrites"][i].GetObject()));
                        }
                    }
                    this->name = GetString(json, "name");
                    this->topic = GetString(json, "topic");
                    this->nsfw = GetBool(json, "nsfw");
                    this->last_message_id = GetString(json, "last_message_id");
                    this->bitrate = GetInt(json, "bitrate");
                    this->user_limit = GetInt(json, "user_limit");
                    this->rate_limit_per_user = GetInt(json, "rate_limit_per_user");
                    if (json.HasMember("recipients")) {
                        for (rapidjson::SizeType i = 0; i < json["recipients"].Size(); i++) {
                            this->recipients.emplace_back(user::user_t(json["recipients"][i].GetObject()));
                        }
                    }
                    this->icon = GetString(json, "icon");
                    this->owner_id = GetString(json, "owner_id");
                    this->application_id = GetString(json, "application_id");
                    this->parent_id = GetString(json, "parent_id");
                    this->last_pin_timestamp = GetString(json, "last_pin_timestamp");
                    this->rtc_region = GetString(json, "rtc_region");
                    this->video_quality_mode = GetInt(json, "video_quality_mode");
                    this->message_count = GetInt(json, "message_count");
                    this->member_count = GetInt(json, "member_count");
                    this->default_auto_archive_duration = GetInt(json, "default_auto_archive_duration");
                    this->permissions = GetString(json, "permissions");
                }

                std::string id;
                channeltype type;
                std::string guild_id;
                uint32_t position;
                std::vector<overwrite::overwrite_t> permission_overwrites;
                std::string name;
                std::string topic;
                bool nsfw;
                std::string last_message_id;
                uint32_t bitrate;
                uint32_t user_limit;
                uint32_t rate_limit_per_user;
                std::vector<user::user_t> recipients;
                std::string icon;
                std::string owner_id;
                std::string application_id;
                std::string parent_id;
                std::string last_pin_timestamp;
                std::string rtc_region;
                uint32_t video_quality_mode;
                uint32_t message_count;
                uint32_t member_count;
                thread::metadata::metadata_t metadata;
                thread::member::member_t member;
                uint32_t default_auto_archive_duration;
                std::string permissions;
            };

            namespace channel_mention {
                struct channel_mention_t {
                    explicit channel_mention_t(rapidjson_object_t json) {
                        this->id = GetString(json, "id");
                        this->guild_id = GetString(json, "guild_id");
                        this->type = static_cast<channeltype>(GetInt(json, "type"));
                        this->name = GetString(json, "name");
                    }

                    std::string id;
                    std::string guild_id;
                    channeltype type = channeltype::guild_text;
                    std::string name;
                };
            }
        }
        namespace guild {
            namespace role {
                namespace tags {
                    struct tags_t {
                        explicit tags_t(rapidjson_object_t json) {
                            this->bot_id = GetString(json, "bot_id");
                            this->integration_id = GetString(json, "integration_id");
                            this->premium_subscriber = GetBool(json, "premium_subscriber");
                        }

                        std::string bot_id;
                        std::string integration_id;
                        bool premium_subscriber = false;
                    };
                }

                struct role_t {
                    explicit role_t(rapidjson_object_t json) : tags(GetObject(json, "tags")) {
                        this->id = GetString(json, "id");
                        this->name = GetString(json, "name");
                        this->color = GetInt(json, "color");
                        this->hoist = GetBool(json, "hoist");
                        this->position = GetInt(json, "position");
                        this->permissions = GetString(json, "permissions");
                        this->managed = GetBool(json, "managed");
                        this->mentionable = GetBool(json, "mentionable");
                    }

                    std::string id;
                    std::string name;
                    uint32_t color = NULL;
                    bool hoist = false;
                    uint32_t position = NULL;
                    std::string permissions;
                    bool managed = false;
                    bool mentionable = false;
                    tags::tags_t tags;
                };
            }
            namespace member {
                struct member_t {
                    explicit member_t(rapidjson_object_t json) : user(GetObject(json, "user")) {
                        this->nick = GetString(json, "nick");
                        if (json.HasMember("roles")) {
                            for (rapidjson::SizeType i = 0; i < json["roles"].Size(); i++) {
                                this->roles.emplace_back(json["roles"][i].GetString());
                            }
                        }
                        this->joined_at = GetString(json, "joined_at");
                        this->premium_since = GetString(json, "premium_since");
                        this->deaf = GetBool(json, "deaf");
                        this->mute = GetBool(json, "mute");
                        this->pending = GetBool(json, "pending");
                        this->permissions = GetString(json, "permissions");
                    }

                    user::user_t user;
                    std::string nick;
                    std::vector<std::string> roles;
                    std::string joined_at;
                    std::string premium_since;
                    bool deaf = false;
                    bool mute = false;
                    bool pending = false;
                    std::string permissions;
                };
            }
        }
        namespace message {
            namespace attachment {
                struct attachment_t {
                    explicit attachment_t(rapidjson_object_t json) {
                        this->id = GetString(json, "id");
                        this->filename = GetString(json, "filename");
                        this->content_type = GetString(json, "content_type");
                        this->size = GetInt(json, "size");
                        this->url = GetString(json, "url");
                        this->proxy_url = GetString(json, "proxy_url");
                        this->height = GetInt(json, "height");
                        this->width = GetInt(json, "width");
                    }

                    std::string id;
                    std::string filename;
                    std::string content_type;
                    uint32_t size = NULL;
                    std::string url;
                    std::string proxy_url;
                    uint32_t height = NULL;
                    uint32_t width = NULL;
                };
            }

            namespace embed {
                namespace footer {
                    struct footer_t {
                        explicit footer_t(rapidjson_object_t json) {
                            this->text = GetString(json, "text");
                            this->icon_url = GetString(json, "icon_url");
                            this->proxy_icon_url = GetString(json, "proxy_icon_url");
                        }

                        std::string text;
                        std::string icon_url;
                        std::string proxy_icon_url;
                    };
                }

                namespace image {
                    struct image_t {
                        explicit image_t(rapidjson_object_t json) {
                            this->url = GetString(json, "url");
                            this->proxy_url = GetString(json, "proxy_url");
                            this->height = GetInt(json, "height");
                            this->width = GetInt(json, "width");
                        }

                        std::string url;
                        std::string proxy_url;
                        uint32_t height = NULL;
                        uint32_t width = NULL;
                    };
                }

                namespace thumbnail {
                    struct thumbnail_t {
                        explicit thumbnail_t(rapidjson_object_t json) {
                            this->url = GetString(json, "url");
                            this->proxy_url = GetString(json, "proxy_url");
                            this->height = GetInt(json, "height");
                            this->width = GetInt(json, " width");
                        }

                        std::string url;
                        std::string proxy_url;
                        uint32_t height = NULL;
                        uint32_t width = NULL;
                    };
                }

                namespace video {
                    struct video_t {
                        explicit video_t(rapidjson_object_t json) {
                            this->url = GetString(json, "url");
                            this->proxy_url = GetString(json, "proxy_url");
                            this->height = GetInt(json, "height");
                            this->width = GetInt(json, " width");
                        }

                        std::string url;
                        std::string proxy_url;
                        uint32_t height = NULL;
                        uint32_t width = NULL;
                    };
                }

                namespace provider {
                    struct provider_t {
                        explicit provider_t(rapidjson_object_t json) {
                            this->name = GetString(json, "name");
                            this->url = GetString(json, "url");
                        }

                        std::string name;
                        std::string url;
                    };
                }

                namespace author {
                    struct author_t {
                        explicit author_t(rapidjson_object_t json) {
                            this->name = GetString(json, "name");
                            this->url = GetString(json, "url");
                            this->icon_url = GetString(json, "icon_url");
                            this->icon_proxy_url = GetString(json, "icon_proxy_url");
                        }

                        std::string name;
                        std::string url;
                        std::string icon_url;
                        std::string icon_proxy_url;
                    };
                }

                namespace field {
                    struct field_t {
                        explicit field_t(rapidjson_object_t json) {
                            this->name = GetString(json, "name");
                            this->value = GetString(json, "value");
                            this->inline_ = GetBool(json, "inline");
                        }

                        std::string name;
                        std::string value;
                        bool inline_;
                    };
                }

                struct embed_t {
                    explicit embed_t(rapidjson_object_t json) : footer(GetObject(json, "footer")), image(GetObject(json, "image")), thumbnail(GetObject(json, "thumbnail")), video(GetObject(json, "video")), provider(GetObject(json, "provider")), author(GetObject(json, "author")) {
                        this->title = GetString(json, "title");
                        this->description = GetString(json, "description");
                        this->url = GetString(json, "url");
                        this->timestamp = GetString(json, "timestamp");
                        this->color = GetInt(json, "color");
                        for (rapidjson::SizeType i = 0; i < json["fields"].Size(); i++) {
                            this->fields.emplace_back(field::field_t(json["fields"][i].GetObject()));
                        }
                    }

                    std::string title;
                    std::string description;
                    std::string url;
                    std::string timestamp;
                    uint32_t color = NULL;
                    footer::footer_t footer;
                    image::image_t image;
                    thumbnail::thumbnail_t thumbnail;
                    video::video_t video;
                    provider::provider_t provider;
                    author::author_t author;
                    std::vector<field::field_t> fields;
                };
            }

            namespace emoji {
                struct emoji_t {
                    explicit emoji_t(rapidjson_object_t json) : user(GetObject(json, "user")) {
                        this->id = GetString(json, "id");
                        this->name = GetString(json, "name");
                        for (rapidjson::SizeType i = 0; i < json["roles"].Size(); i++) {
                            this->roles.emplace_back(json["roles"][i].GetString());
                        }
                        this->require_colons = GetBool(json, "require_colons");
                        this->managed = GetBool(json, "managed");
                        this->animated = GetBool(json, "animated");
                        this->available = GetBool(json, "available");
                    }

                    std::string id;
                    std::string name;
                    std::vector<std::string> roles;
                    user::user_t user;
                    bool require_colons;
                    bool managed;
                    bool animated;
                    bool available;
                };
            }

            namespace reaction {
                struct reaction_t {
                    explicit reaction_t(rapidjson_object_t json) : emoji(GetObject(json, "emoji")) {
                        this->count = GetInt(json, "count");
                        this->me = GetBool(json, "me");
                    }

                    uint32_t count;
                    bool me;
                    emoji::emoji_t emoji;
                };
            }

            enum class messagetype : uint32_t {
                default_,
                recipient_add,
                recipient_remove,
                call,
                channel_name_change,
                channel_icon_change,
                channel_pinned_message,
                guild_member_join,
                user_premium_guild_subscription,
                user_premium_guild_subscription_tier_1,
                user_premium_guild_subscription_tier_2,
                user_premium_guild_subscription_tier_3,
                channel_follow_add,
                guild_discovery_disqualified = 14,
                guild_discovery_requalified,
                guild_discovery_grace_period_initial_warning,
                guild_discovery_grace_period_final_warning,
                thread_created,
                reply,
                application_command,
                thread_starter_message,
                guild_invite_reminder,
            };

            namespace reference {
                struct reference_t {
                    explicit reference_t(rapidjson_object_t json) {
                        this->message_id = GetString(json, "message_id");
                        this->channel_id = GetString(json, "channel_id");
                        this->guild_id = GetString(json, "guild_id");
                        this->fail_if_not_exists = GetBool(json, "fail_if_not_exists");
                    }

                    std::string message_id;
                    std::string channel_id;
                    std::string guild_id;
                    bool fail_if_not_exists;
                };
            }

            enum class flags : uint32_t {
                crossposted = 1 << 0,
                is_crosspost = 1 << 1,
                suppress_embeds = 1 << 2,
                source_message_deleted = 1 << 3,
                urgent = 1 << 4,
                has_thread = 1 << 5,
                ephemeral = 1 << 6,
                loading = 1 << 7,
            };

            namespace interaction {
                enum class interactiontype : uint32_t {
                    ping = 1,
                    application_command,
                    message_component,
                };

                struct interaction_t {
                    explicit interaction_t(rapidjson_object_t json) : user(GetObject(json, "user")) {
                        this->id = GetString(json, "id");
                        this->type = static_cast<interactiontype>(GetInt(json, "type"));
                        this->name = GetString(json, "name");
                    }

                    std::string id;
                    interactiontype type;
                    std::string name;
                    user::user_t user;
                };
            }

            namespace component {
                namespace option {
                    struct option_t {
                        explicit option_t(rapidjson_object_t json) : emoji(GetObject(json, "emoji")) {
                            this->label = GetString(json, "label");
                            this->value = GetString(json, "value");
                            this->description = GetString(json, "description");
                            this->default_ = GetBool(json, "default");
                        }

                        std::string label;
                        std::string value;
                        std::string description;
                        emoji::emoji_t emoji;
                        bool default_;
                    };
                }

                enum class componenttype {
                    actionrow = 1,
                    button,
                    selectmenu,
                };

                enum class styles {
                    primary = 1,
                    secondary,
                    success,
                    danger,
                    link,
                };

                struct child_component_t {
                    explicit child_component_t(rapidjson_object_t json) : emoji(GetObject(json, "emoji")) {
                        this->type = static_cast<componenttype>(GetInt(json, "type"));
                        this->custom_id = GetString(json, "custom_id");
                        this->disabled = GetBool(json, "disabled");
                        this->style = static_cast<styles>(GetInt(json, "style"));
                        this->label = GetString(json, "label");
                        this->url = GetString(json, "url");
                        for (rapidjson::SizeType i = 0; i < json["options"].Size(); i++) {
                            this->options.emplace_back(option::option_t(json["options"][i].GetObject()));
                        }
                        this->placeholder = GetString(json, "placeholder");
                        this->min_values = GetInt(json, "min_values");
                        this->max_values = GetInt(json, "max_values");
                    }

                    componenttype type;
                    std::string custom_id;
                    bool disabled;
                    styles style;
                    std::string label;
                    emoji::emoji_t emoji;
                    std::string url;
                    std::vector<option::option_t> options;
                    std::string placeholder;
                    uint32_t min_values;
                    uint32_t max_values;
                };

                struct component_t {
                    explicit component_t(rapidjson_object_t json) : emoji(GetObject(json, "emoji")) {
                        this->type = static_cast<componenttype>(GetInt(json, "type"));
                        this->custom_id = GetString(json, "custom_id");
                        this->disabled = GetBool(json, "disabled");
                        this->style = static_cast<styles>(GetInt(json, "style"));
                        this->label = GetString(json, "label");
                        this->url = GetString(json, "url");
                        for (rapidjson::SizeType i = 0; i < json["options"].Size(); i++) {
                            this->options.emplace_back(option::option_t(json["options"][i].GetObject()));
                        }
                        this->placeholder = GetString(json, "placeholder");
                        this->min_values = GetInt(json, "min_values");
                        this->max_values = GetInt(json, "max_values");
                        for (rapidjson::SizeType i = 0; i < json["components"].Size(); i++) {
                            this->components.emplace_back(child_component_t(json["components"][i].GetObject()));
                        }
                    }

                    componenttype type;
                    std::string custom_id;
                    bool disabled;
                    styles style;
                    std::string label;
                    emoji::emoji_t emoji;
                    std::string url;
                    std::vector<option::option_t> options;
                    std::string placeholder;
                    uint32_t min_values;
                    uint32_t max_values;
                    std::vector<child_component_t> components;
                };
            }

            namespace sticker {
                enum class stickerformat : uint32_t {
                    png = 1,
                    apng,
                    lottie,
                };

                struct item_t {
                    explicit item_t(rapidjson_object_t json) {
                        this->id = GetString(json, "id");
                        this->name = GetString(json, "name");
                        this->format = static_cast<stickerformat>(GetInt(json, "format"));
                    }

                    std::string id;
                    std::string name;
                    stickerformat format;
                };

                enum class stickertype : uint32_t {
                    standard = 1,
                    guild,
                };

                struct sticker_t {
                    explicit sticker_t(rapidjson_object_t json) : user(GetObject(json, "user")){
                        this->id = GetString(json, "id");
                        this->pack_id = GetString(json, "pack_id");
                        this->name = GetString(json, "name");
                        this->description = GetString(json, "description");
                        this->tags = GetString(json, "tags");
                        this->asset = GetString(json, "asset");
                        this->type = static_cast<stickertype>(GetInt(json, "type"));
                        this->format = static_cast<stickerformat>(GetInt(json, "format"));
                        this->available = GetBool(json, "available");
                        this->guild_id = GetString(json, "guild_id");
                        this->sort_value = GetInt(json, "sort_value");
                    }

                    std::string id;
                    std::string pack_id;
                    std::string name;
                    std::string description;
                    std::string tags;
                    std::string asset;
                    stickertype type;
                    stickerformat format;
                    bool available;
                    std::string guild_id;
                    user::user_t user;
                    uint32_t sort_value;
                };
            }

            struct message_t {
                explicit message_t(rapidjson_object_t json) : author(GetObject(json, "author")), member(GetObject(json, "member")), message_reference(GetObject(json, "message_reference")), interaction(GetObject(json, "interaction")), thread(GetObject(json, "thread"))  {
                    this->id = GetString(json, "id");
                    this->channel_id = GetString(json, "channel_id");
                    this->guild_id = GetString(json, "guild_id");
                    this->content = GetString(json, "content");
                    this->timestamp = GetString(json, "timestamp");
                    this->edited_timestamp = GetString(json, "edited_timestamp");
                    this->tts = GetBool(json, "tts");
                    this->mention_everyone = GetBool(json, "mention_everyone");
                    if (json.HasMember("mentions")) {
                        for (rapidjson::SizeType i = 0; i < json["mentions"].Size(); i++) {
                            this->mentions.emplace_back(user::user_t(json["mentions"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("mention_roles")) {
                        for (rapidjson::SizeType i = 0; i < json["mention_roles"].Size(); i++) {
                            this->mention_roles.emplace_back(json["mention_roles"][i].GetString());
                        }
                    }
                    if (json.HasMember("mention_channels")) {
                        for (rapidjson::SizeType i = 0; i < json["mention_channels"].Size(); i++) {
                            this->mention_channels.emplace_back(channel::channel_mention::channel_mention_t(json["mention_channels"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("attachments")) {
                        for (rapidjson::SizeType i = 0; i < json["attachments"].Size(); i++) {
                            this->attachments.emplace_back(attachment::attachment_t(json["attachments"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("embeds")) {
                        for (rapidjson::SizeType i = 0; i < json["embeds"].Size(); i++) {
                            this->embeds.emplace_back(embed::embed_t(json["embeds"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("reactions")) {
                        for (rapidjson::SizeType i = 0; i < json["reactions"].Size(); i++) {
                            this->reactions.emplace_back(reaction::reaction_t(json["reactions"][i].GetObject()));
                        }
                    }
                    this->pinned = GetBool(json, "pinned");
                    this->webhook_id = GetString(json, "webhook_id");
                    this->type = static_cast<messagetype>(GetInt(json, "type"));
                    this->application_id = GetString(json, "application_id");
                    this->flags = GetInt(json, "flags");
                    if (json.HasMember("components")) {
                        for (rapidjson::SizeType i = 0; i < json["components"].Size(); i++) {
                            this->components.emplace_back(component::component_t(json["components"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("sticker_items")) {
                        for (rapidjson::SizeType i = 0; i < json["sticker_items"].Size(); i++) {
                            this->sticker_items.emplace_back(sticker::item_t(json["sticker_items"][i].GetObject()));
                        }
                    }
                    if (json.HasMember("stickers")) {
                        for (rapidjson::SizeType i = 0; i < json["stickers"].Size(); i++) {
                            this->stickers.emplace_back(sticker::sticker_t(json["stickers"][i].GetObject()));
                        }
                    }
                }

                std::string id;
                std::string channel_id;
                std::string guild_id;
                user::user_t author;
                guild::member::member_t member;
                std::string content;
                std::string timestamp;
                std::string edited_timestamp;
                bool tts;
                bool mention_everyone;
                std::vector<user::user_t> mentions;
                std::vector<std::string> mention_roles;
                std::vector<channel::channel_mention::channel_mention_t> mention_channels;
                std::vector<attachment::attachment_t> attachments;
                std::vector<embed::embed_t> embeds;
                std::vector<reaction::reaction_t> reactions;
                bool pinned;
                std::string webhook_id;
                messagetype type;
                std::string application_id;
                reference::reference_t message_reference;
                uint32_t flags;
                interaction::interaction_t interaction;
                channel::channel_t thread;
                std::vector<component::component_t> components;
                std::vector<sticker::item_t> sticker_items;
                std::vector<sticker::sticker_t> stickers;
            };
        }
    }
}
#endif //DISCORD_CPP_BOT_DATATYPES_HPP
