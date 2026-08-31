################################################################################
# \file Makefile
# \version 1.0
#
# \brief
# Top-level application make file.
#
################################################################################
# \copyright
# $ Copyright 2026-YEAR Infineon Technologies AG Apache2 $
################################################################################

################################################################################
# Basic Configuration
################################################################################

MTB_TYPE=APPLICATION

MTB_PROJECTS=proj_cm0p proj_cm4

include common_app.mk

include $(CY_TOOLS_DIR)/make/application.mk
