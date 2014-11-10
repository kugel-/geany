/* Scintilla source code edit control */
/** @file ScintillaWidget.h
 ** Definition of Scintilla widget for GTK+.
 ** Only needed by GTK+ code but is harmless on other platforms.
 **/
/* Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
 * The License.txt file describes the conditions under which this software may be distributed. */

#ifndef SCINTILLAWIDGET_H
#define SCINTILLAWIDGET_H

#if defined(GTK)

#ifndef SCI_EXPORT
#define SCI_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Type macros.
 */
#define SCINTILLA_TYPE_OBJECT             (scintilla_object_get_type())
#define SCINTILLA_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), SCINTILLA_TYPE_OBJECT, ScintillaObject))
#define SCINTILLA_IS_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCINTILLA_TYPE_OBJECT))
#define SCINTILLA_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), SCINTILLA_TYPE_OBJECT, ScintillaObjectClass))
#define SCINTILLA_IS_OBJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), SCINTILLA_TYPE_OBJECT))
#define SCINTILLA_OBJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), SCINTILLA_TYPE_OBJECT, ScintillaObjectClass))

typedef struct _ScintillaObject          ScintillaObject;
typedef struct _ScintillaObjectClass     ScintillaObjectClass;

struct _ScintillaObject {
	GtkContainer cont;
	void *pscin;
};

struct _ScintillaObjectClass {
	GtkContainerClass parent_class;

	void (* command)	(gint sci_id, void *notif);
	void (* notify)		(gint sci_id, void *notif);
};



GType		scintilla_object_get_type (void);
GtkWidget*	scintilla_object_new (void);
void		scintilla_object_set_id			(ScintillaObject *sci, uptr_t id);
long		scintilla_object_send_message	(ScintillaObject *sci,unsigned int iMessage,
											 unsigned long wParam, long lParam);

void		scintilla_release_resources(void);

#define SCINTILLA_NOTIFY "sci-notify"

#ifdef __cplusplus
}
#endif

/* include compat header for now, remove this after deprecation period */
#include "ScintillaWidgetCompat.h"

#endif /* GTK */

#endif
