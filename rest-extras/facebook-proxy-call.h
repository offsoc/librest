/*
 * librest - RESTful web services access
 * Copyright (c) 2008, 2009, Intel Corporation.
 *
 * Authors: Rob Bradford <rob@linux.intel.com>
 *          Ross Burton <ross@linux.intel.com>
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef _FACEBOOK_PROXY_CALL
#define _FACEBOOK_PROXY_CALL

#include <rest/rest-proxy-call.h>

G_BEGIN_DECLS

#define FACEBOOK_TYPE_PROXY_CALL facebook_proxy_call_get_type()

#define FACEBOOK_PROXY_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), FACEBOOK_TYPE_PROXY_CALL, FacebookProxyCall))

#define FACEBOOK_PROXY_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), FACEBOOK_TYPE_PROXY_CALL, FacebookProxyCallClass))

#define FACEBOOK_IS_PROXY_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FACEBOOK_TYPE_PROXY_CALL))

#define FACEBOOK_IS_PROXY_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), FACEBOOK_TYPE_PROXY_CALL))

#define FACEBOOK_PROXY_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), FACEBOOK_TYPE_PROXY_CALL, FacebookProxyCallClass))

/**
 * FacebookProxyCall:
 *
 * #FacebookProxyCall has no publicly available members.
 */
typedef struct {
  RestProxyCall parent;
} FacebookProxyCall;

typedef struct {
  RestProxyCallClass parent_class;
  /*< private >*/
  /* padding for future expansion */
  gpointer _padding_dummy[8];
} FacebookProxyCallClass;

GType facebook_proxy_call_get_type (void);

G_END_DECLS

#endif /* _FACEBOOK_PROXY_CALL */