#ifndef CSS_DATA_H
#define CSS_DATA_H

static const char* CLASS_DEFAULT_TEXT_VIEW = "default-text-view";

static const char* CLASS_ERROR = "error-label";
static const char* CLASS_INVIS = "invis";
static const char* CLASS_VIS = "vis";
static const char* CLASS_STATUS_BAR = "status-bar";

static const char* MAIN_STYLESHEET = " \
.default-text-view { \
    padding: 8px; \
    border-radius: 8px; \
} \
\
.error-label { \
    font-weight: bold; \
    color:rgb(224, 83, 83); \
} \
\
.invis { \
    opacity: 0; \
    transition: 0.2s; \
} \
\
.vis { \
    opacity: 1; \
    transition: 0.2s; \
} \
.status-bar { \
    border-top: 1px solid rgba(131, 131, 131, 0.4); \
    border-bottom: 1px solid rgba(131, 131, 131, 0.4); \
    padding: 4px 0px 4px 0px; \
} \
";

#endif
