/* Scintilla source code edit control */
/* Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
 * The License.txt file describes the conditions under which this software may be distributed. */

#ifndef SCINTILLAWIDGETCOMPAT_H
#define SCINTILLAWIDGETCOMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The following definitions help keeping backwards compatibility for clients.
 *
 * All of this is deprecated and may be removed in the future, so you should
 * transition to the new naming scheme (or copy this header into your code base).
 */

typedef ScintillaObjectClass ScintillaClass;

#define SCINTILLA(x) SCINTILLA_OBJECT(x)
#define IS_SCINTILLA(x) SCINTILLA_IS_OBJECT(x)

GType		scintilla_get_type		(void);
GtkWidget*	scintilla_new			(void);
void		scintilla_set_id		(ScintillaObject *sci, uptr_t id);
sptr_t		scintilla_send_message	(ScintillaObject *sci,unsigned int iMessage, uptr_t wParam, sptr_t lParam);

#ifdef __cplusplus
}
#endif


#endif
