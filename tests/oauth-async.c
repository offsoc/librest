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

#include <rest/oauth-proxy.h>
#include <rest/oauth-proxy-call.h>
#include <rest/oauth-proxy-private.h>
#include <stdio.h>
#include <stdlib.h>

static GMainLoop *loop;

static void
make_calls (OAuthProxy *oproxy)
{
  RestProxy *proxy = REST_PROXY (oproxy);
  RestProxyCall *call;
  GError *error = NULL;

  /* Make some test calls */

  call = rest_proxy_new_call (proxy);
  rest_proxy_call_set_function (call, "echo");
  rest_proxy_call_add_param (call, "foo", "bar");
  if (!rest_proxy_call_sync (call, &error))
    g_error ("Cannot make call: %s", error->message);
  g_assert_cmpstr (rest_proxy_call_get_payload (call), ==, "foo=bar");
  g_object_unref (call);

  call = rest_proxy_new_call (proxy);
  rest_proxy_call_set_function (call, "echo");
  rest_proxy_call_add_param (call, "numbers", "1234567890");
  if (!rest_proxy_call_sync (call, &error))
    g_error ("Cannot make call: %s", error->message);
  g_assert_cmpstr (rest_proxy_call_get_payload (call), ==, "numbers=1234567890");
  g_object_unref (call);

  call = rest_proxy_new_call (proxy);
  rest_proxy_call_set_function (call, "echo");
  rest_proxy_call_add_param (call, "escape", "!£$%^&*()");
  if (!rest_proxy_call_sync (call, &error))
    g_error ("Cannot make call: %s", error->message);
  g_assert_cmpstr (rest_proxy_call_get_payload (call), ==, "escape=%21%C2%A3%24%25%5E%26%2A%28%29");
  g_object_unref (call);

  g_main_loop_quit (loop);
}

static void
access_token_cb (GObject      *source_object,
                 GAsyncResult *result,
                 gpointer      user_data)
{
  OAuthProxyCall *call = OAUTH_PROXY_CALL (source_object);
  OAuthProxy *proxy;
  OAuthProxyPrivate *priv;
  GError *error = NULL;

  rest_proxy_call_invoke_finish (REST_PROXY_CALL (call), result, &error);
  g_assert_no_error (error);

  g_object_get (G_OBJECT (call), "proxy", &proxy, NULL);
  priv = PROXY_GET_PRIVATE (proxy);

  g_assert_cmpstr (priv->token, ==, "accesskey");
  g_assert_cmpstr (priv->token_secret, ==, "accesssecret");

  make_calls (proxy);
}

static void
request_token_cb (GObject      *source_object,
                  GAsyncResult *result,
                  gpointer      user_data)
{
  OAuthProxyCall *call = OAUTH_PROXY_CALL (source_object);
  OAuthProxy *proxy;
  OAuthProxyPrivate *priv;
  GError *error = NULL;

  rest_proxy_call_invoke_finish (REST_PROXY_CALL (call), result, &error);
  g_assert_no_error (error);

  g_object_get (G_OBJECT (call), "proxy", &proxy, NULL);
  priv = PROXY_GET_PRIVATE (proxy);

  g_assert_cmpstr (priv->token, ==, "requestkey");
  g_assert_cmpstr (priv->token_secret, ==, "requestsecret");

  /* Second stage authentication, this gets an access token */
  oauth_proxy_access_token_async (proxy, "access-token", NULL,
                                  NULL, access_token_cb, NULL);
}

static gboolean
on_timeout (gpointer data)
{
  exit (1);
  return FALSE;
}

int
main (int argc, char **argv)
{
  RestProxy *proxy;
  OAuthProxy *oproxy;

  /* Install a timeout so that we don't hang or infinite loop */
  g_timeout_add_seconds (10, on_timeout, NULL);

  loop = g_main_loop_new (NULL, TRUE);

  /* Create the proxy */
  proxy = oauth_proxy_new ("key", "secret",
                           "http://oauthbin.com/v1/",
                           FALSE);
  oproxy = OAUTH_PROXY (proxy);
  g_assert (oproxy);

  /* First stage authentication, this gets a request token */
  oauth_proxy_request_token_async (oproxy, "request-token", NULL,
                                   NULL, request_token_cb, loop);
  g_main_loop_run (loop);

  return 0;
}
