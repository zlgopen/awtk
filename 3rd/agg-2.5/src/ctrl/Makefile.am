if ENABLE_CTRL
INCLUDES = -I$(top_srcdir)/include

noinst_LTLIBRARIES = libaggctrl.la

libaggctrl_la_LDFLAGS = -no-undefined -version-info @AGG_LIB_VERSION@
libaggctrl_la_SOURCES = agg_cbox_ctrl.cpp agg_gamma_ctrl.cpp agg_gamma_spline.cpp agg_rbox_ctrl.cpp \
						agg_slider_ctrl.cpp agg_spline_ctrl.cpp agg_scale_ctrl.cpp \
						agg_bezier_ctrl.cpp agg_polygon_ctrl.cpp 

endif
