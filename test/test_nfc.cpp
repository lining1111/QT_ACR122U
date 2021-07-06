//
// Created by lining on 7/6/21.
//

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif // HAVE_CONFIG_H

#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <nfc/nfc.h>

#include "nfc/nfc-utils.h"

#define MAX_DEVICE_COUNT 16
#define MAX_TARGET_COUNT 16

static nfc_device *pnd;

static void
print_usage(const char *progname) {
    printf("usage: %s [-v] [-t X]\n", progname);
    printf("  -v\t verbose display\n");
    printf("  -t X\t poll only for types according to bitfield X:\n");
    printf("\t   1: ISO14443A\n");
    printf("\t   2: Felica (212 kbps)\n");
    printf("\t   4: Felica (424 kbps)\n");
    printf("\t   8: ISO14443B\n");
    printf("\t  16: ISO14443B'\n");
    printf("\t  32: ISO14443B-2 ST SRx\n");
    printf("\t  64: ISO14443B-2 ASK CTx\n");
    printf("\t 128: ISO14443B iClass\n");
    printf("\t 256: ISO14443A-3 Jewel\n");
    printf("\t 512: ISO14443A-2 NFC Barcode\n");
    printf("\tSo 1023 (default) polls for all types.\n");
    printf("\tNote that if 16, 32, 64 or 128 then 8 is selected too.\n");
}

int
main(int argc, const char *argv[]) {
    (void) argc;
    const char *acLibnfcVersion;
    size_t i;
    bool verbose = false;
    int res = 0;
    int mask = 0x3ff;
    int arg;

    nfc_context *context;
    nfc_init(&context);
    if (context == NULL) {
        ERR("Unable to init libnfc (malloc)");
        exit(EXIT_FAILURE);
    }

    // Get commandline options
    for (arg = 1; arg < argc; arg++) {
        if (0 == strcmp(argv[arg], "-h")) {
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        } else if (0 == strcmp(argv[arg], "-v")) {
            verbose = true;
        } else if ((0 == strcmp(argv[arg], "-t")) && (arg + 1 < argc)) {
            arg++;
            mask = atoi(argv[arg]);
            if ((mask < 1) || (mask > 0x3ff)) {
                ERR("%i is invalid value for type bitfield.", mask);
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            // Force TypeB for all derivatives of B
            if (mask & 0xf0)
                mask |= 0x08;
        } else {
            ERR("%s is not supported option.", argv[arg]);
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Display libnfc version
    if (verbose) {
        acLibnfcVersion = nfc_version();
        printf("%s uses libnfc %s\n", argv[0], acLibnfcVersion);
    }


    /* Lazy way to open an NFC device */
#if 0
    pnd = nfc_open(context, NULL);
#endif

    /* Use connection string if specific device is wanted,
     * i.e. PN532 UART device on /dev/ttyUSB1 */
#if 0
    pnd = nfc_open(context, "pn532_uart:/dev/ttyUSB1");
#endif

    nfc_connstring connstrings[MAX_DEVICE_COUNT];
    size_t szDeviceFound = nfc_list_devices(context, connstrings, MAX_DEVICE_COUNT);

    if (szDeviceFound == 0) {
        printf("No NFC device found.\n");
    }

    for (i = 0; i < szDeviceFound; i++) {
        nfc_target ant[MAX_TARGET_COUNT];
        pnd = nfc_open(context, connstrings[i]);

        if (pnd == NULL) {
            ERR("Unable to open NFC device: %s", connstrings[i]);
            continue;
        }
        if (nfc_initiator_init(pnd) < 0) {
            nfc_perror(pnd, "nfc_initiator_init");
            nfc_exit(context);
            exit(EXIT_FAILURE);
        }

        printf("NFC device: %s opened\n", nfc_device_get_name(pnd));

        nfc_modulation nm;

        if (mask & 0x1) {
            nm.nmt = NMT_ISO14443A;
            nm.nbr = NBR_106;
            // List ISO14443A targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443A passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x02) {
            nm.nmt = NMT_FELICA;
            nm.nbr = NBR_212;
            // List Felica tags
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d Felica (212 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x04) {
            nm.nmt = NMT_FELICA;
            nm.nbr = NBR_424;
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d Felica (424 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x08) {
            nm.nmt = NMT_ISO14443B;
            nm.nbr = NBR_106;
            // List ISO14443B targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443B passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x10) {
            nm.nmt = NMT_ISO14443BI;
            nm.nbr = NBR_106;
            // List ISO14443B' targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443B' passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x20) {
            nm.nmt = NMT_ISO14443B2SR;
            nm.nbr = NBR_106;
            // List ISO14443B-2 ST SRx family targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443B-2 ST SRx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x40) {
            nm.nmt = NMT_ISO14443B2CT;
            nm.nbr = NBR_106;
            // List ISO14443B-2 ASK CTx family targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443B-2 ASK CTx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x80) {
            nm.nmt = NMT_ISO14443BICLASS;
            nm.nbr = NBR_106;
            // List ISO14443B iClass targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443B iClass passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x100) {
            nm.nmt = NMT_JEWEL;
            nm.nbr = NBR_106;
            // List Jewel targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443A-3 Jewel passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        if (mask & 0x200) {
            nm.nmt = NMT_BARCODE;
            nm.nbr = NBR_106;
            // List NFC Barcode targets
            if ((res = nfc_initiator_list_passive_targets(pnd, nm, ant, MAX_TARGET_COUNT)) >= 0) {
                int n;
                if (verbose || (res > 0)) {
                    printf("%d ISO14443A-2 NFC Barcode passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
                }
                for (n = 0; n < res; n++) {
                    print_nfc_target(&ant[n], verbose);
                    printf("\n");
                }
            }
        }

        nfc_close(pnd);
    }

    nfc_exit(context);
    exit(EXIT_SUCCESS);
}