/*
 *      actioncallbacks.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2006-2012 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
 *      Copyright 2006-2012 Nick Treleaven <nick(dot)treleaven(at)btinternet(dot)com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "dialogs.h"
#include "document.h"
#include "editor.h"
#include "main.h"
#include "printing.h"
#include "project.h"
#include "sciwrappers.h"
#include "sidebar.h"
#include "support.h"
#include "utils.h"
#include "ui_utils.h"

#include <glib.h>
#include <gtk/gtk.h>


G_MODULE_EXPORT void
on_file_new_action_activate(GtkAction *action, gpointer user_data)
{
	document_new_file(NULL, NULL, NULL);
}


G_MODULE_EXPORT void
on_file_open_action_activate(GtkAction *action, gpointer user_data)
{
	dialogs_show_open_file();
}


G_MODULE_EXPORT void
on_file_openselected_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();
	gchar *sel = NULL;
	const gchar *wc;

#ifdef G_OS_WIN32
	wc = GEANY_WORDCHARS "./-" "\\";
#else
	wc = GEANY_WORDCHARS "./-";
#endif

	g_return_if_fail(doc != NULL);

	sel = editor_get_default_selection(doc->editor, TRUE, wc);
	SETPTR(sel, utils_get_locale_from_utf8(sel));

	if (sel != NULL)
	{
		gchar *filename = NULL;

		if (g_path_is_absolute(sel))
			filename = g_strdup(sel);
		else
		{	/* relative filename, add the path of the current file */
			gchar *path;

			path = utils_get_current_file_dir_utf8();
			SETPTR(path, utils_get_locale_from_utf8(path));
			if (!path)
				path = g_get_current_dir();

			filename = g_build_path(G_DIR_SEPARATOR_S, path, sel, NULL);

			if (! g_file_test(filename, G_FILE_TEST_EXISTS) &&
				app->project != NULL && !EMPTY(app->project->base_path))
			{
				/* try the project's base path */
				SETPTR(path, project_get_base_path());
				SETPTR(path, utils_get_locale_from_utf8(path));
				SETPTR(filename, g_build_path(G_DIR_SEPARATOR_S, path, sel, NULL));
			}
			g_free(path);
#ifdef G_OS_UNIX
			if (! g_file_test(filename, G_FILE_TEST_EXISTS))
				SETPTR(filename, g_build_path(G_DIR_SEPARATOR_S, "/usr/local/include", sel, NULL));

			if (! g_file_test(filename, G_FILE_TEST_EXISTS))
				SETPTR(filename, g_build_path(G_DIR_SEPARATOR_S, "/usr/include", sel, NULL));
#endif
		}

		if (g_file_test(filename, G_FILE_TEST_EXISTS))
			document_open_file(filename, FALSE, NULL, NULL);
		else
		{
			SETPTR(sel, utils_get_utf8_from_locale(sel));
			ui_set_statusbar(TRUE, _("Could not open file %s (File not found)"), sel);
		}

		g_free(filename);
		g_free(sel);
	}
}


G_MODULE_EXPORT void
on_file_save_action_activate(GtkAction *action, gpointer user_data)
{
	gint cur_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(main_widgets.notebook));
	GeanyDocument *doc = document_get_current();

	if (doc != NULL && cur_page >= 0)
	{
		document_save_file(doc, ui_prefs.allow_always_save);
	}
}


G_MODULE_EXPORT void
on_file_saveas_action_activate(GtkAction *action, gpointer user_data)
{
	dialogs_show_save_as();
}


G_MODULE_EXPORT void
on_file_saveall_action_activate(GtkAction *action, gpointer user_data)
{
	guint i, max = (guint) gtk_notebook_get_n_pages(GTK_NOTEBOOK(main_widgets.notebook));
	GeanyDocument *doc, *cur_doc = document_get_current();
	guint count = 0;

	/* iterate over documents in tabs order */
	for (i = 0; i < max; i++)
	{
		doc = document_get_from_page(i);
		if (! doc->changed)
			continue;

		if (document_save_file(doc, FALSE))
			count++;
	}
	if (!count)
		return;

	ui_set_statusbar(FALSE, ngettext("%d file saved.", "%d files saved.", count), count);
	/* saving may have changed window title, sidebar for another doc, so update */
	document_show_tab(cur_doc);
	sidebar_update_tag_list(cur_doc, TRUE);
	ui_set_window_title(cur_doc);
}


G_MODULE_EXPORT void
on_file_print_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();
	g_return_if_fail(doc != NULL);

	printing_print_doc(doc);
}


G_MODULE_EXPORT void
on_file_close_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(DOC_VALID(doc));

	document_close(doc);
}


G_MODULE_EXPORT void
on_file_closeall_action_activate(GtkAction *action, gpointer user_data)
{
	document_close_all();
}


G_MODULE_EXPORT void
on_file_reload_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	document_reload_prompt(doc, NULL);
}


G_MODULE_EXPORT void
on_file_openlasttab_action_activate(GtkAction *action, gpointer user_data)
{
	gchar *utf8_filename = g_queue_peek_head(ui_prefs.recent_queue);
	gchar *locale_filename = utils_get_locale_from_utf8(utf8_filename);

	document_open_file(locale_filename, FALSE, NULL, NULL);
	g_free(locale_filename);
}


G_MODULE_EXPORT void
on_file_quit_action_activate(GtkAction *action, gpointer user_data)
{
	main_quit();
}


G_MODULE_EXPORT void
on_project_new_action_activate(GtkAction *action, gpointer user_data)
{
	project_new();
}


G_MODULE_EXPORT void
on_project_open_action_activate(GtkAction *action, gpointer user_data)
{
	project_open();
}


G_MODULE_EXPORT void
on_project_properties_action_activate(GtkAction *action, gpointer user_data)
{
	project_properties();
}


G_MODULE_EXPORT void
on_project_close_action_activate(GtkAction *action, gpointer user_data)
{
	project_close(TRUE);
}


G_MODULE_EXPORT void
on_editor_undo_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	if (document_can_undo(doc))
	{
		sci_cancel(doc->editor->sci);
		document_undo(doc);
	}
}


G_MODULE_EXPORT void
on_editor_redo_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	if (document_can_redo(doc))
	{
		sci_cancel(doc->editor->sci);
		document_redo(doc);
	}
}


G_MODULE_EXPORT void
on_editor_duplicateline_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	if (sci_get_lines_selected(doc->editor->sci) > 1)
	{	/* ignore extra_line because of selecting lines from the line number column */
		editor_select_lines(doc->editor, FALSE);
		sci_selection_duplicate(doc->editor->sci);
	}
	else if (sci_has_selection(doc->editor->sci))
		sci_selection_duplicate(doc->editor->sci);
	else
		sci_line_duplicate(doc->editor->sci);
}


G_MODULE_EXPORT void
on_editor_deleteline_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	editor_select_lines(doc->editor, TRUE); /* include last line (like cut lines, copy lines do) */
	sci_clear(doc->editor->sci);	/* (SCI_LINEDELETE only does 1 line) */
}


G_MODULE_EXPORT void
on_editor_deletelinetoend_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	sci_send_command(doc->editor->sci, SCI_DELLINERIGHT);
}


G_MODULE_EXPORT void
on_editor_transposeline_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	sci_send_command(doc->editor->sci, SCI_LINETRANSPOSE);
}


G_MODULE_EXPORT void
on_editor_scrolltoline_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	editor_scroll_to_line(doc->editor, -1, 0.5F);
}


G_MODULE_EXPORT void
on_editor_scrolllineup_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	sci_send_command(doc->editor->sci, SCI_LINESCROLLUP);
}


G_MODULE_EXPORT void
on_editor_scrolllinedown_action_activate(GtkAction *action, gpointer user_data)
{
	GeanyDocument *doc = document_get_current();

	g_return_if_fail(doc != NULL);

	sci_send_command(doc->editor->sci, SCI_LINESCROLLDOWN);
}


G_MODULE_EXPORT void
on_editor_completesnippet_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_snippetnextcursor_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_suppresssnippetcompletion_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_contextaction_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_autocomplete_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_calltip_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_macrolist_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_wordpartcompletion_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_movelineup_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_movelinedown_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_clipboard_cut_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_clipboard_copy_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_clipboard_paste_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_clipboard_copyline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_clipboard_cutline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_all_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_word_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_line_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_paragraph_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_wordpartleft_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_select_wordpartright_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_togglecase_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_commentlinetoggle_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_commentline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_uncommentline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_increaseindent_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_decreaseindent_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_increaseindentbyspace_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_decreaseindentbyspace_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_autoindent_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_sendtocmd1_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_sendtocmd2_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_sendtocmd3_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_sendtovte_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_format_reflowparagraph_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_insert_date_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_insert_altwhitespace_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_insert_linebefore_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_insert_lineafter_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_settings_preferences_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_settings_pluginpreferences_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_find_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findnext_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findprevious_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findnextsel_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findprevsel_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_replace_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findinfiles_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_nextmessage_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_previousmessage_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_findusage_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_finddocumentusage_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_search_markall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_back_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_forward_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_line_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_matchingbrace_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_togglemarker_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_nextmarker_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_previousmarker_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_tagdefinition_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_tagdeclaration_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_linestart_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_lineend_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_linestartvisual_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_lineendvisual_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_prevwordpart_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_goto_nextwordpart_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_toggleall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_fullscreen_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_messagewindow_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_sidebar_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_zoomin_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_zoomout_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_view_zoomreset_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_editor_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_searchbar_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_message_window_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_compiler_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_messages_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_scribble_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_vte_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_sidebar_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_sidebar_symbol_list_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_focus_sidebar_document_list_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_switchtableft_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_switchtabright_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_switchtablastused_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_movetableft_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_movetabright_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_movetabfirst_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_notebook_movetablast_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_linewrap_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_linebreak_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_clone_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_replacetabs_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_replacespaces_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_togglefold_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_foldall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_unfoldall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_reloadtaglist_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_remove_markers_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_remove_error_indicators_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_document_remove_markers_indicators_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_compile_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_link_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_make_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_makeowntarget_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_makeobject_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_nexterror_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_previouserror_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_run_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_build_options_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_tools_opencolorchooser_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_help_help_action_activate(GtkAction *action, gpointer user_data)
{
}
