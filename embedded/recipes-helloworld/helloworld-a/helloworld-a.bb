SUMMARY = "helloworld of Autotools recipe"
DESCRIPTION = "My hello world application"
LICENSE = "CLOSED"

inherit autotools

FILESEXTRAPATHS_prepend := "${THISDIR}/:"
SRC_URI = "file://helloworld/"

S = "${WORKDIR}/helloworld"

#CFLAGS_append = "-Wall -O -g"

FILES_${PN} = "${bindir}/"
