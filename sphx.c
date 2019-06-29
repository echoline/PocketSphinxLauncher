/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 1999-2010 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*
 * sphx.c; Eli Cohen, June 2012
 * based on:
 */
/*
 * continuous.c - Simple pocketsphinx command-line application to test
 *                both continuous listening/silence filtering from microphone
 *                and continuous file transcription.
 */

/*
 * This is a simple example of pocketsphinx application that uses continuous listening
 * with silence filtering to automatically segment a continuous stream of audio input
 * into utterances that are then decoded.
 * 
 * Remarks:
 *   - Each utterance is ended when a silence segment of at least 1 sec is recognized.
 *   - Single-threaded implementation for portability.
 *   - Uses audio library; can be replaced with an equivalent custom library.
 */

#include <stdio.h>
#include <string.h>

#define _OPEN_THREAD
#include <pthread.h>

#if !defined(_WIN32_WCE)
#include <signal.h>
#include <setjmp.h>
#endif
#if defined(WIN32) && !defined(GNUWINCE)
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>

#include <pocketsphinx.h>

//extern char *adcdev;
extern char *modeldir;
extern char *hmmdir;
extern char *lmdump;
extern char *lmdict;

static ps_decoder_t *ps;
static cmd_ln_t *config;
static ad_rec_t *ad;

/* Sleep for specified msec */
static void
sleep_msec(int32 ms)
{
#if (defined(WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

/*
 * Main utterance processing loop:
 *     for (;;) {
 * 	   wait for start of next utterance;
 * 	   decode utterance until silence of at least 1 sec observed;
 * 	   print utterance result;
 *     }
 */
static void
recognize_from_microphone(int outfd)
{
    int16 adbuf[4096];
    int32 k, ts, rem;
    char const *hyp;
    char const *uttid;
    char word[4096];
    uint8 in_speech, utt_started;

    if ((ad = ad_open_dev(NULL,
                          (int)cmd_ln_float32_r(config, "-adcdev"))) == NULL)
        E_FATAL("Failed to open audio device\n");

    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");
    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");

    utt_started = FALSE;
    /* Indicate listening for next utterance */
    write(outfd, "READY\n", 6);

    for (;;) {
        /* Wait data for next utterance */
        while ((k = ad_read(ad, adbuf, 2048)) == 0)
            sleep_msec(100);

        if (k < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
	in_speech = ps_get_in_speech(ps);
       	if (in_speech && !utt_started) {
            utt_started = TRUE;
	    write(outfd, "Listening\n", 10);
        }
	if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                dprintf(outfd, "%s\n", hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
	    write(outfd, "READY\n", 10);
        }
        sleep_msec(100);
    }
    ad_close(ad);

    fprintf (stderr, "listen thread exiting\n");
}

void*
sphinx_gui_listen_main(void *arg)
{
    int outfd = (int)arg;
    char const *cfg;
    char hmm[256];
    char lm[256];
    char dict[256];

    snprintf(hmm, 256, "%s/%s", modeldir, hmmdir);
    snprintf(lm, 256, "%s/%s", modeldir, lmdump);
    snprintf(dict, 256, "%s/%s", modeldir, lmdict);

    config = cmd_ln_init(NULL, ps_args(), TRUE,
			"-hmm", hmm, "-lm", lm, "-dict", dict,
			NULL);

    if (config == NULL)
        return NULL;

    ps = ps_init(config);
    if (ps == NULL)
        return NULL;

    recognize_from_microphone(outfd);

    ps_free(ps);

    return NULL;
}
