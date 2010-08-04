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

#include <string.h>
#include <libsoup/soup.h>
#include <rest/rest-proxy-call.h>
#include "flickr-proxy-call.h"
#include "flickr-proxy-private.h"
#include "rest/rest-proxy-call-private.h"
#include "rest/sha1.h"

G_DEFINE_TYPE (FlickrProxyCall, flickr_proxy_call, REST_TYPE_PROXY_CALL)

static gboolean
_prepare (RestProxyCall *call, GError **error)
{
  FlickrProxy *proxy = NULL;
  FlickrProxyPrivate *priv;
  RestProxyCallPrivate *call_priv;
  GHashTable *params;
  char *s;

  g_object_get (call, "proxy", &proxy, NULL);
  priv = FLICKR_PROXY_GET_PRIVATE (proxy);
  call_priv = call->priv;

  /* First reset the URL because Flickr puts the function in the parameters */
  call_priv->url = g_strdup ("http://api.flickr.com/services/rest/");

  rest_proxy_call_add_params (call,
                              "method", call_priv->function,
                              "api_key", priv->api_key,
                              NULL);

  if (priv->token)
    rest_proxy_call_add_param (call, "auth_token", priv->token);

  /* Get the string params as a hash for signing */
  params = rest_params_as_string_hash_table (call_priv->params);
  s = flickr_proxy_sign (proxy, params);
  g_hash_table_unref (params);

  rest_proxy_call_add_param (call, "api_sig", s);
  g_free (s);

  g_object_unref (proxy);

  return TRUE;
}

static void
flickr_proxy_call_class_init (FlickrProxyCallClass *klass)
{
  RestProxyCallClass *call_class = REST_PROXY_CALL_CLASS (klass);

  call_class->prepare = _prepare;
}

static void
flickr_proxy_call_init (FlickrProxyCall *self)
{
}

#if BUILD_TESTS
#warning TODO flickr signature test cases
#endif
