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
}


G_MODULE_EXPORT void
on_file_save_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_saveas_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_saveall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_print_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_close_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_closeall_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_reload_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_openlasttab_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_file_quit_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_project_new_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_project_open_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_project_properties_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_project_close_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_undo_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_redo_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_duplicateline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_deleteline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_deletelinetoend_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_transposeline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_scrolltoline_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_scrolllineup_action_activate(GtkAction *action, gpointer user_data)
{
}


G_MODULE_EXPORT void
on_editor_scrolllinedown_action_activate(GtkAction *action, gpointer user_data)
{
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
