/*
 * uoproxy
 * $Id$
 *
 * (c) 2005 Max Kellermann <max@duempel.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "packets.h"

struct selectx;
struct instance;

struct item {
    struct item *next;
    u_int32_t serial;
    struct uo_packet_put packet_put;
};

struct mobile {
    struct mobile *next;
    u_int32_t serial;
    struct uo_packet_mobile_incoming *packet_mobile_incoming;
    struct uo_packet_mobile_status *packet_mobile_status;
};

struct connection {
    /* linked list and parent */
    struct connection *next;
    struct instance *instance;

    /* flags */
    int invalid, background, attaching, welcome;

    /* state */
    char username[30], password[30];
    u_int16_t supported_features_flags;
    struct uo_packet_start packet_start;
    struct uo_packet_map_change packet_map_change;
    struct uo_packet_map_patches packet_map_patches;
    struct uo_packet_season packet_season;
    struct uo_packet_mobile_update packet_mobile_update;
    struct uo_packet_global_light_level packet_global_light_level;
    struct uo_packet_personal_light_level packet_personal_light_level;
    struct uo_packet_war_mode packet_war_mode;
    unsigned char ping_request, ping_ack;
    struct item *items_head;
    struct mobile *mobiles_head;

    /* sub-objects */
    struct uo_client *client;
    struct uo_server *server;
};

struct instance {
    /* login server */
    u_int32_t login_ip;
    u_int16_t login_port;

    /* state */
    struct connection *connections_head;
    struct relay_list *relays;
};

int connection_new(struct instance *instance,
                   int server_socket,
                   struct connection **connectionp);

void connection_delete(struct connection *c);

void connection_invalidate(struct connection *c);

void connection_pre_select(struct connection *c, struct selectx *sx);

int connection_post_select(struct connection *c, struct selectx *sx);

void connection_idle(struct connection *c);

void connection_add_item(struct connection *c,
                         const struct uo_packet_put *p);

void connection_mobile_incoming(struct connection *c,
                                const struct uo_packet_mobile_incoming *p);

void connection_mobile_status(struct connection *c,
                              const struct uo_packet_mobile_status *p);

void attach_after_game_login(struct connection *old,
                             struct connection *new);

void attach_after_play_character(struct connection *c);

#endif
