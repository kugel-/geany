/*
 *  Copyright 2012 Colomban Wendling  <ban(at)herbesfolles(dot)org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

[CCode(cheader_fileanme="geany.h",
       cname="Geany",
       gir_namespace="GeanyRaw",
       gir_version="1.0")]
namespace GeanyRaw
{
	[Compact]
	public class App {
		public bool								debug_mode;
		public string							configdir;
		public string							datadir;
		public string							docdir;
		public unowned TagManager.Workspace		tm_workspace;
		public unowned Project?					project;
	}
	/* Reviewed */
	[Compact]
	public class InterfacePrefs {
		public bool		sidebar_symbol_visible;
		public bool		sidebar_openfiles_visible;
		public string	editor_font;
		public string	tagbar_font;
		public string	msgwin_font;
		public bool		show_notebook_tabs;
		public int		tab_pos_editor;
		public int		tab_pos_msgwin;
		public int		tab_pos_sidebar;
		public bool		statusbar_visible;
		public bool		show_symbol_list_expanders;
		public bool		notebook_double_click_hides_widgets;
		public bool		highlighting_invert_all;
		public int		sidebar_pos;
		public bool		msgwin_status_visible;
		public bool		msgwin_compiler_visible;
		public bool		msgwin_messages_visible;
		public bool		msgwin_scribble_visible;
		public bool		use_native_windows_dialogs; /* only used on Windows */
		public bool		compiler_tab_autoscroll;
	}
	[Compact]
	public class IndentPrefs {
		public int width;
		public IndentType type;
		public int hard_tab_width;
		public AutoIndent auto_indent_mode;
		public bool detect_type;
	}
	[Compact]
	public class EditorPrefs {
		public IndentPrefs	indentation;
		public bool			show_white_space;
		public bool			show_indent_guide;
		public bool			show_line_endings;
		public int			long_line_type;
		public int			long_line_column;
		public string		long_line_color;
		public bool			show_markers_margin;		/* view menu */
		public bool			show_linenumber_margin;		/* view menu */
		public bool			show_scrollbars;			/* hidden pref */
		public bool			scroll_stop_at_last_line;
		public bool			line_wrapping;
		public bool			use_indicators;
		public bool			folding;
		public bool			unfold_all_children;
		public bool			disable_dnd;
		public bool			use_tab_to_indent;	/* makes tab key indent instead of insert a tab char */
		public bool			smart_home_key;
		public bool			newline_strip;
		public bool			auto_complete_symbols;
		public bool			auto_close_xml_tags;
		public bool			complete_snippets;
		public int			symbolcompletion_min_chars;
		public int			symbolcompletion_max_height;
		public bool			brace_match_ltgt;	/* whether to highlight < and > chars (hidden pref) */
		public bool			use_gtk_word_boundaries;	/* hidden pref */
		public bool			complete_snippets_whilst_editing;	/* hidden pref */
		public int			line_break_column;
		public bool			auto_continue_multiline;
		public string		comment_toggle_mark;
		public uint			autocompletion_max_entries;
		public uint			autoclose_chars;
		public bool			autocomplete_doc_words;
		public bool			completion_drops_rest_of_word;
		public string		color_scheme;
		public int			show_virtual_space;
		public bool			long_line_enabled;
		public int			autocompletion_update_freq;

	}
	/* reviewed */
	[Compact]
	public class ToolbarPrefs {
		public bool				visible;
		public Gtk.IconSize		icon_size;
		public Gtk.ToolbarStyle	icon_style;
		public bool				use_gtk_default_style;
		public bool				use_gtk_default_icon;
		public bool				append_to_menu;
	}
	/* reviewed */
	[Compact]
	public class Prefs {
		public bool		load_session;
		public bool		load_plugins;
		public bool		save_winpos;
		public bool		confirm_exit;
		public bool		beep_on_errors;		/* use utils_beep() instead */
		public bool		suppress_status_messages;
		public bool		switch_to_status;
		public bool		auto_focus;
		public string	default_open_path;
		public string	custom_plugin_path;
	}
	/* reviewed */
	[Compact]
	public class ToolPrefs {
		public string	browser_cmd;
		public string	term_cmd;
		public string	grep_cmd;
		public string	context_action_cmd;
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_FIND_SEL_", has_type_id = false)]
	public enum FindSelOptions {
		CURRENT_WORD,
		SEL_X,
		AGAIN
	}
	[Compact]
	public class SearchPrefs {
		public bool				suppress_dialogs;
		public bool				use_current_word;
		public bool				use_current_file_dir;
		public bool				hide_find_dialog;
		public FindSelOptions	find_selection_type;
	}
	/* reviewed
	 * FIXME: are (some of) these nullable? */
	[Compact]
	public class TemplatePrefs {
		public string	developer;
		public string	company;
		public string	mail;
		public string	initials;
		public string	version;
		public string	year_format;
		public string	date_format;
		public string	datetime_format;
	}
	/* reviewed */
	[Compact]
	public class FilePrefs {
		public int		default_new_encoding;
		public int		default_open_encoding;
		public bool		final_new_line;
		public bool		strip_trailing_spaces;
		public bool		replace_tabs;
		public bool		tab_order_ltr;
		public bool		tab_order_beside;
		public bool		show_tab_cross;
		public uint		mru_length;
		public int		default_eol_character;
		public int		disk_check_timeout;
		public bool		cmdline_new_files;
		public bool		use_safe_file_saving;
		public bool		ensure_convert_new_lines;
		public bool		gio_unsafe_save_backup;
		public string 	extract_filetype_regex;
		public bool		tab_close_switch_to_mru;
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_GBG_", has_type_id = false)]
	public enum BuildGroup {
		FT,
		NON_FT,
		EXEC,
		COUNT
	}
	[Compact]
	public class BuildInfo {
		BuildGroup		grp;
		int				cmd;
		GLib.Pid		pid;
		string			dir;
		uint			file_type_id;
		string			custom_target;
		int				message_count;
	}
	/* reviewed */
	[Compact]
	public class Data {
		public App						app;
		public MainWidgets				main_widgets;
		public GLib.PtrArray			documents_array;
		public GLib.PtrArray			filetypes_array;
		public Prefs					prefs;
		public InterfacePrefs			interface_prefs;
		public ToolbarPrefs				toolbar_prefs;
		public EditorPrefs				editor_prefs;
		public FilePrefs				file_prefs;
		public SearchPrefs				search_prefs;
		public ToolPrefs				tool_prefs;
		public TemplatePrefs			template_prefs;
		public BuildInfo				build_info;
		public GLib.SList<Filetype>		filetypes_by_title;
	}
	/* reviewed */
	[CCode (cprefix = "dialogs_")]
	namespace Dialogs {
		public string?	show_input (string title, Gtk.Window parent,
									string? label_text = null, string? default_text = null);
		public bool		show_input_numeric (string title, string label_text,
											ref double value, double min, double max, double step);
		[PrintfFormat]
		public void		show_msgbox (Gtk.MessageType type, string text, ...);
		[PrintfFormat]
		public bool		show_question (string text, ...);
		public bool		show_save_as ();
	}
	[Compact]
	[CCode(cheader_filename="document.h",
	       cname="struct GeanyDocument",
	       cprefix="document_",
	       free_function="",
	       has_type_id=false)]
	public class Document
	{
		public bool						is_valid;
		public int						index;
		public bool						has_tags;
		public string?					file_name;
		public string					encoding;
		public bool						has_bom;
		public Editor					editor;
		public Filetype					file_type;
		public TagManager.WorkObject	tm_file;
		public bool						readonly;
		public bool						changed;
		public string?					real_path;

		public void set_text_changed(bool changed);
		public void set_encoding(string encoding);
		[CCode (cname = "DOC_FILENAME")]
		public string doc_filename();
		public void set_filetype(Filetype ft);
		[CCode (cname = "DOC_VALID")]
		public bool doc_valid();
		public int get_notebook_page();
		public unowned Gdk.Color? get_status_color();
		public string get_basename_for_display(int length = -1);
		public static void open_files (GLib.SList<string> filenames,
									   bool readonly = false,
									   Filetype? ft = null,
									   string? forced_enc = null);
		public bool reload_file (string? forced_enc = null);
		public void rename_file (string new_filename);
		public bool save_file (bool force = false);
		public bool save_file_as (string? utf8_fname = null);

		public static unowned Document? new_file (string? utf8_filename = null,
												  Filetype? ft = null,
												  string? text = null);
		public bool close ();
		public static bool remove_page (uint page_num);
		public static unowned Document? find_by_filename (string utf8_filename);
		public static unowned Document? find_by_real_path (string real_name);
		public static unowned Document? get_current ();
		public static unowned Document? get_from_page (uint page_num);
		/* renamed method because of index field */
		[CCode (cname = "document_index")]
		public static unowned Document? get_by_index (int index);
		public static unowned Document? open_file (string locale_filename,
												   bool readonly = false,
												   Filetype? ft = null, 
												   string? forced_enc = null);
	}
	/* reviewed */
	[CCode (cname = "documents", array_length_cexpr = "GEANY(documents_array)->len")]
	public static unowned Document[]	documents;
	/* reviewed */
	[CCode (cprefix = "GEANY_INDICATOR_", has_type_id = false)]
	public enum Indicator {
		ERROR,
		SEARCH
	}
	[CCode (cname = "GEANY_WORDCHARS")]
	public static const string WORDCHARS;
	[Compact]
	[CCode(cheader_filename="editor.h",
	       cname="struct GeanyEditor",
	       cprefix="editor_",
	       free_function="",
	       has_type_id=false)]
	public class Editor {
		public Document				document;
		public Scintilla.Object		sci;
		public bool					line_wrapping;
		public bool					auto_indent;
		public float				scroll_percent;
		public IndentType			indent_type;
		public bool					line_breaking;
		public int					indent_width;

		public Scintilla.Object create_widget(Editor editor) {}

		private void set_indent_type (IndentType ind_type);

		public static unowned string get_eol_char(Editor? editor = null);
		public static int get_eol_char_mode(Editor? editor = null);
		public static unowned string get_eol_char_name(Editor? editor = null);
		public static unowned IndentPrefs get_indent_prefs(Editor? editor = null);
		public unowned string? find_snippet (string snippet_name);
		public string? get_word_at_pos (int pos, string? wordchars = null);
		public bool goto_pos (int pos, bool mark = false);
		public void indicator_clear (Indicator indic);
		public void indicator_set_on_line (Indicator indic, int line);
		public void indicator_set_on_range (Indicator indic, int start, int end);
		public void insert_snippet (int pos, string snippet);
		public void insert_text_block (string text,  int insert_pos,
									   int cursor_index = -1, int newline_indent_size = -1,
									   bool replace_newlines = true);
	}
	/* reviewed */
	[CCode (cname = "GeanyEncodingIndex", cprefix = "GEANY_ENCODING_", has_type_id = false)]
	public enum EncodingID {
		ISO_8859_1,
		ISO_8859_2,
		ISO_8859_3,
		ISO_8859_4,
		ISO_8859_5,
		ISO_8859_6,
		ISO_8859_7,
		ISO_8859_8,
		ISO_8859_8_I,
		ISO_8859_9,
		ISO_8859_10,
		ISO_8859_13,
		ISO_8859_14,
		ISO_8859_15,
		ISO_8859_16,

		UTF_7,
		UTF_8,
		UTF_16LE,
		UTF_16BE,
		UCS_2LE,
		UCS_2BE,
		UTF_32LE,
		UTF_32BE,

		ARMSCII_8,
		BIG5,
		BIG5_HKSCS,
		CP_866,

		EUC_JP,
		EUC_KR,
		EUC_TW,

		GB18030,
		GB2312,
		GBK,
		GEOSTD8,
		HZ,

		IBM_850,
		IBM_852,
		IBM_855,
		IBM_857,
		IBM_862,
		IBM_864,

		ISO_2022_JP,
		ISO_2022_KR,
		ISO_IR_111,
		JOHAB,
		KOI8_R,
		KOI8_U,

		SHIFT_JIS,
		TCVN,
		TIS_620,
		UHC,
		VISCII,

		WINDOWS_1250,
		WINDOWS_1251,
		WINDOWS_1252,
		WINDOWS_1253,
		WINDOWS_1254,
		WINDOWS_1255,
		WINDOWS_1256,
		WINDOWS_1257,
		WINDOWS_1258,

		NONE,
		CP_932
	}
	public enum EncodingGroup
	{
		NONE,
		WESTEUROPEAN,
		EASTEUROPEAN,
		EASTASIAN,
		ASIAN,
		MIDDLEEASTERN,
		UNICODE
	}
	/* reviewed */
	[Compact]
	[CCode (cprefix = "encodings_")]
	public class Encoding {
		public EncodingID				idx;
		public int						order;
		public EncodingGroup			group;
		public unowned string			charset;
		public unowned string			name;
		
		public static string?			convert_to_utf8 (string buffer, size_t size,
														 out string used_encoding = null);
		public static string?			convert_to_utf8_from_charset (string buffer, size_t size,
																	  string charset, bool fast);
		public static unowned string?	get_charset_from_index (EncodingID idx);
	}
	/* reviewed */
	[CCode (cname = "filetype_id", cprefix = "GEANY_FILETYPES_", has_type_id = false)]
	public enum FiletypeID {
		NONE,
		
		PHP,
		BASIC,
		MATLAB,
		RUBY,
		LUA,
		FERITE,
		YAML,
		C,
		NSIS,
		GLSL,
		PO,
		MAKE,
		TCL,
		XML,
		CSS,
		REST,
		HASKELL,
		JAVA,
		CAML,
		AS,
		R,
		DIFF,
		HTML,
		PYTHON,
		CS,
		PERL,
		VALA,
		PASCAL,
		LATEX,
		ASM,
		CONF,
		HAXE,
		CPP,
		SH,
		FORTRAN,
		SQL,
		F77,
		DOCBOOK,
		D,
		JS,
		VHDL,
		ADA,
		CMAKE,
		MARKDOWN,
		TXT2TAGS,
		ABC,
		VERILOG,
		FORTH,
		LISP,
		ERLANG,
		COBOL,
		/* ^ append items here */
		[CCode (cname = "GEANY_MAX_BUILT_IN_FILETYPES")]
		MAX_BUILT_IN_FILETYPES
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_FILETYPE_GROUP_", has_type_id = false)]
	public enum FiletypeGroupID
	{
		NONE,
		COMPILED,
		SCRIPT,
		MARKUP,
		MISC,
		CUSTOM,
		COUNT
	}
	/* reviewed */
	[Compact]
	[CCode(cheader_filename="filetypes.h",
	       cname="struct GeanyFiletype",
	       cprefix="filetypes_",
	       free_function="",
	       has_type_id=false)]
	public class Filetype {
		public FiletypeID				id;
		public TagManager.LangType		lang;
		public string					name;
		public string					title;
		public string?					extension;
		[CCode (array_length = false, array_null_terminated = true)]
		public string[]					pattern;
		public string					context_action_cmd;
		public string?					comment_open;
		public string?					comment_close;
		public bool						comment_use_indent;
		public FiletypeGroupID			group;
		public string?					error_regex_string;
		public Filetype					lexer_filetype;
		public string?					mime_type;
		public GLib.Icon?				icon;
		public string					comment_single;
		public int						indent_type;
		public int						indent_width;
		
		public unowned string			get_display_name();
		
		public static unowned Filetype	detect_from_file (string utf8_filename);
		public static unowned Filetype?	index (int idx);
		public static unowned Filetype?	lookup_by_name (string name);
	}
	[CCode(cname="filetypes_array")]
	GLib.PtrArray filetypes_array;
	/* reviewed */
	[CCode (cname = "filetypes_by_title")]
	public GLib.SList<Filetype> filetypes_by_title;
	[CCode(cheader_filename="geanyobject.h", cname="GeanyObject")]
	public class Object : GLib.Object
	{
		// Signals
		public signal void document_new(Document doc);
		public signal void document_open(Document doc);
		public signal void document_reload(Document doc);
		public signal void document_before_save(Document doc);
		public signal void document_save(Document doc);
		public signal void document_filetype_set(Document doc, Filetype old_ft);
		public signal void document_activate(Document doc);
		public signal void document_close(Document doc);

		public signal void project_open(GLib.KeyFile config);
		public signal void project_save(GLib.KeyFile config);
		public signal void project_close();
		public signal void project_dialog_open(Gtk.Notebook nb);
		public signal void project_dialog_confirmed(Gtk.Notebook nb);
		public signal void project_dialog_close(Gtk.Notebook nb);

		[CCode (cname = "geany_startup_complete")]
		public signal void startup_complete();

		public signal void build_start();
		
		public signal void update_editor_menu(string word, int pos, Document doc);
		
		public signal bool editor_notify(Editor editor, Scintilla.Notification nt);
	}
	/* TODO check */
	[CCode (cname="geany_object", type="GeanyObject *")]
	public GeanyRaw.Object geany_object;
	/* reviewed */
	[Compact]
	public class Functions {
		/* No need to fill-in, the functions are wrapped.
		 * However, the plugins needs to define geany_functions with this type,
		 * so we need to have it here. */
	}
	/* reviewed */
	[CCode (lower_case_cprefix = "highlighting_")]
	namespace Highlighting {
		public unowned LexerStyle	get_style (int ft_id, int style_id);
		public bool					is_code_style (int lexer, int style);
		public bool					is_comment_style (int lexer, int style);
		public bool					is_string_style (int lexer, int style);
		public void					set_styles (Scintilla.Object sci, Filetype ft);
	}
	/* reviewed */
	[CCode (has_target = false)]
	public delegate void KeyCallback (uint key_id);
	[Compact]
	[CCode (has_type_id = false)]
	public class KeyBinding {} /* public in geany but want to make it opaque */
	/* reviewed */
	[CCode (lower_case_cprefix = "keybindings_")]
	namespace Keybindings {
		public unowned KeyBinding	get_item (KeyGroup group, size_t key_id);
		public void					send_command (KeyGroupID group_id, KeyBindingID key_id);
		public unowned KeyBinding	set_item (KeyGroup group, size_t key_id, KeyCallback? cb,
											  uint key, Gdk.ModifierType mod, string kf_name,
											  string label, Gtk.Widget? menu_item = null);
	}
	/* reviewed */
	[Compact]
	[CCode (has_type_id = false)]
	public class KeyGroup {}
	/* reviewed */
	[CCode (has_target = false)]
	public delegate bool KeyGroupCallback (uint key_id);
	/* reviewed */
	[CCode (cprefix = "GEANY_KEYS_", has_type_id = false)]
	public enum KeyBindingID {
		EDITOR_TRANSPOSELINE,
		DOCUMENT_REMOVE_ERROR_INDICATORS,
		FOCUS_SEARCHBAR,
		SEARCH_FIND,
		FILE_SAVEALL,
		GOTO_NEXTMARKER,
		NOTEBOOK_SWITCHTABLEFT,
		VIEW_ZOOMOUT,
		GOTO_LINE,
		DOCUMENT_TOGGLEFOLD,
		BUILD_COMPILE,
		EDITOR_SCROLLTOLINE,
		DOCUMENT_UNFOLDALL,
		GOTO_MATCHINGBRACE,
		SEARCH_FINDDOCUMENTUSAGE,
		CLIPBOARD_PASTE,
		BUILD_MAKE,
		INSERT_ALTWHITESPACE,
		EDITOR_SCROLLLINEDOWN,
		VIEW_TOGGLEALL,
		VIEW_FULLSCREEN,
		GOTO_LINEEND,
		EDITOR_CALLTIP,
		FILE_PRINT,
		EDITOR_DUPLICATELINE,
		FOCUS_SCRIBBLE,
		TOOLS_OPENCOLORCHOOSER,
		SEARCH_PREVIOUSMESSAGE,
		FILE_CLOSE,
		DOCUMENT_REPLACETABS,
		FILE_RELOAD,
		SEARCH_FINDNEXTSEL,
		FOCUS_MESSAGES,
		BUILD_RUN,
		HELP_HELP,
		SETTINGS_PLUGINPREFERENCES,
		VIEW_ZOOMRESET,
		SELECT_WORD,
		FORMAT_INCREASEINDENT,
		SETTINGS_PREFERENCES,
		FORMAT_SENDTOCMD3,
		DOCUMENT_FOLDALL,
		FORMAT_SENDTOVTE,
		PROJECT_PROPERTIES,
		DOCUMENT_LINEWRAP,
		EDITOR_MACROLIST,
		EDITOR_SUPPRESSSNIPPETCOMPLETION,
		FOCUS_SIDEBAR_SYMBOL_LIST,
		GOTO_LINESTART,
		SEARCH_FINDUSAGE,
		FILE_NEW,
		EDITOR_SNIPPETNEXTCURSOR,
		NOTEBOOK_SWITCHTABRIGHT,
		FILE_SAVE,
		FORMAT_INCREASEINDENTBYSPACE,
		SEARCH_FINDNEXT,
		GOTO_TOGGLEMARKER,
		GOTO_TAGDEFINITION,
		SEARCH_NEXTMESSAGE,
		EDITOR_DELETELINETOEND,
		FORMAT_AUTOINDENT,
		FILE_OPENSELECTED,
		GOTO_BACK,
		INSERT_DATE,
		BUILD_PREVIOUSERROR,
		GOTO_LINEENDVISUAL,
		DOCUMENT_REPLACESPACES,
		FOCUS_EDITOR,
		SELECT_WORDPARTRIGHT,
		VIEW_MESSAGEWINDOW,
		FOCUS_SIDEBAR_DOCUMENT_LIST,
		FORMAT_REFLOWPARAGRAPH,
		EDITOR_MOVELINEUP,
		NOTEBOOK_MOVETABLEFT,
		SELECT_LINE,
		EDITOR_UNDO,
		EDITOR_MOVELINEDOWN,
		CLIPBOARD_COPYLINE,
		BUILD_MAKEOWNTARGET,
		FORMAT_SENDTOCMD2,
		SEARCH_MARKALL,
		BUILD_LINK,
		FILE_CLOSEALL,
		GOTO_FORWARD,
		CLIPBOARD_CUT,
		NOTEBOOK_SWITCHTABLASTUSED,
		NOTEBOOK_MOVETABRIGHT,
		BUILD_OPTIONS,
		GOTO_TAGDECLARATION,
		FILE_OPEN,
		EDITOR_COMPLETESNIPPET,
		FORMAT_UNCOMMENTLINE,
		FOCUS_VTE,
		FORMAT_SENDTOCMD1,
		SELECT_WORDPARTLEFT,
		VIEW_ZOOMIN,
		DOCUMENT_LINEBREAK,
		EDITOR_REDO,
		EDITOR_CONTEXTACTION,
		SEARCH_FINDPREVSEL,
		FORMAT_DECREASEINDENTBYSPACE,
		FORMAT_COMMENTLINETOGGLE,
		SELECT_ALL,
		DOCUMENT_RELOADTAGLIST,
		BUILD_NEXTERROR,
		NOTEBOOK_MOVETABLAST,
		SELECT_PARAGRAPH,
		EDITOR_DELETELINE,
		CLIPBOARD_COPY,
		VIEW_SIDEBAR,
		FILE_SAVEAS,
		FORMAT_COMMENTLINE,
		GOTO_PREVWORDPART,
		SEARCH_FINDPREVIOUS,
		SEARCH_REPLACE,
		EDITOR_WORDPARTCOMPLETION,
		EDITOR_AUTOCOMPLETE,
		FOCUS_SIDEBAR,
		FOCUS_MESSAGE_WINDOW,
		NOTEBOOK_MOVETABFIRST,
		GOTO_PREVIOUSMARKER,
		EDITOR_SCROLLLINEUP,
		FOCUS_COMPILER,
		FORMAT_TOGGLECASE,
		CLIPBOARD_CUTLINE,
		DOCUMENT_REMOVE_MARKERS,
		BUILD_MAKEOBJECT,
		FORMAT_DECREASEINDENT,
		FILE_OPENLASTTAB,
		SEARCH_FINDINFILES,
		GOTO_NEXTWORDPART,
		INSERT_LINEAFTER,
		INSERT_LINEBEFORE,
		DOCUMENT_REMOVE_MARKERS_INDICATORS,
		PROJECT_OPEN,
		PROJECT_NEW,
		PROJECT_CLOSE,
		FORMAT_JOINLINES,
		GOTO_LINESTARTVISUAL,
		DOCUMENT_CLONE,
		FILE_QUIT,
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_KEY_GROUP_", has_type_id = false)]
	public enum KeyGroupID {
		FILE,
		PROJECT,
		EDITOR,
		CLIPBOARD,
		SELECT,
		FORMAT,
		INSERT,
		SETTINGS,
		SEARCH,
		GOTO,
		VIEW,
		FOCUS,
		NOTEBOOK,
		DOCUMENT,
		BUILD,
		TOOLS,
		HELP
	}
	/* reviewed */
	[Compact]
	public class LexerStyle {
		public int	foreground;
		public int	background;
		public bool	bold;
		public bool	italic;
	}
	/* reviewed */
	[CCode (lower_case_cprefix = "main_")]
	namespace Main {
		public void		reload_configuration ();
		public void		locale_init (string locale_dir, string package);
		public bool		is_realized ();
	}
	[Compact]
	public class MainWidgets {
		Gtk.Widget		window;
		Gtk.Widget		toolbar;
		Gtk.Widget		sidebar_notebook;
		Gtk.Widget		notebook;
		Gtk.Widget		editor_menu;
		Gtk.Widget		tools_menu;
		Gtk.Widget		progressbar;
		Gtk.Widget		message_window_notebook;
		Gtk.Widget		project_menu;
	}
	[CCode (lower_case_cprefix = "msgwin_", cprefix = "msgwin_")]
	namespace MessageWindow {
		[CCode (cname = "MessageWindowTabNum", has_type_id = false)]
		public enum TabNum {
			STATUS,
			COMPILER,
			MESSAGE,
			SCRATCH,
			VTE
		}
		[CCode (has_type_id = false)]
		public enum MsgColors {
			RED,
			DARK_RED,
			BLACK,
			BLUE
		}

		public void		clear_tab (MessageWindow.TabNum tabnum);
		[PrintfFormat]
		public void		compiler_add (MessageWindow.MsgColors msg_color, string format, ...);
		[PrintfFormat]
		public void		msg_add (MessageWindow.MsgColors msg_color, int line, Document? doc, string format, ...);
		public void		set_messages_dir (string messages_dir);
		[PrintfFormat]
		public void		status_add (string format, ...);
		public void		switch_tab (MessageWindow.TabNum tabnum, bool show);
	}
	/* reviewed */
	[CCode (lower_case_cprefix = "navqueue_")]
	namespace NavQueue {
		public bool		goto_line (Document? old_doc, Document new_doc, int line);
	}
	[Compact]
	[CCode (lower_case_cprefix = "plugin_")]
	public class Plugin {
		[Compact]
		[CCode (cname = "PluginInfo")]
		public class Info {
			public unowned string name;
			public unowned string? description;
			public unowned string? version;
			public unowned string? author;
			
			[CCode (cname = "geany_vapi_SET_INFO")]
			public void @set (string name, string? description, string? version, string? author)
			{
				this.name			= name;
				this.description	= description;
				this.version		= version;
				this.author			= author;
			}
		}
		[Compact]
		[CCode (cname = "PluginCallback")]
		public class Callback {
			public unowned string	signal_name;
			public GLib.Callback	@callback;
			public bool				after;
			public void				*user_data;
		}
		
		[CCode (cname = "geany_vapi_VERSION_CHECK")]
		public static int version_check (int abi_version, int api_required) {
			/* drop-in copy of GEANY_VERSION_CHECK() macro */
			if (abi_version != ABI_VERSION)
				return -1;
			return (api_required);
		}
		
		[CCode (cname = "GEANY_API_VERSION")]
		static int API_VERSION;
		[CCode (cname = "GEANY_ABI_VERSION")]
		static int ABI_VERSION;
		
		public Info info;
		
		public void		add_toolbar_item (Gtk.ToolItem item);
		public void		module_make_resident ();
		public void		signal_connect (GLib.Object? object, string signal_name, bool after,
										GLib.Callback cb, void *user_data = null);
		public unowned KeyGroup	set_key_group (string section_name, size_t count, KeyGroupCallback? cb);
		public void		show_configure ();
	}
	/* reviewed */
	[Compact]
	public class Project {
		public string	base_path;
		public string	description;
		public string	file_name;
		[CCode (array_length = false, array_null_terminated = true)]
		public string[]	file_patterns;
		public string	name;
		public int		type;
	}
	/* reviewed */
	[CCode (cprefix = "search_")]
	namespace Search {
		public void		show_find_in_files_dialog (string? dir = null);
	}
	/* reviewed */
	[Compact]
	[CCode (cname = "StashGroup", cprefix = "stash_group_", free_function = "stash_group_free")]
	public class StashGroup {
		public StashGroup (string name);
		public void		add_boolean (ref bool setting, string key_name, bool default_value);
		public void		add_combo_box (ref int setting, string key_name, int default_value, void *widget_id);
		public void		add_combo_box_entry (ref string setting, string key_name, string default_value,
											 void *widget_id);
		public void		add_entry (ref string setting, string key_name, string default_value,
								   void *widget_id);
		public void		add_integer (ref int setting, string key_name, int default_value);
		public void		add_radio_buttons (ref int setting, string key_name, int default_value,
										   void *widget_id, int enum_id, ...);
		public void		add_spin_button_integer (ref int setting, string key_name, int default_value,
												 void *widget_id);
		public void		add_string (ref string setting, string key_name, string? default_value);
		public void		add_string_vector (ref string[] setting, string key_name, string[]? default_value);
		public void		add_toggle_button (ref bool setting, string key_name, bool default_value,
										   void *widget_id);
		public void		add_widget_property (void *setting, string key_name, void *default_value,
											 void *widget_id, string property_name, GLib.Type type);
		public void		display (Gtk.Widget? owner);
		public bool		load_from_file (string filename);
		public void		load_from_key_file (GLib.KeyFile keyfile);
		public int		save_to_file (string filename, GLib.KeyFileFlags flags = GLib.KeyFileFlags.NONE);
		public void		save_to_key_file (GLib.KeyFile keyfile);
		public void		update (Gtk.Widget? owner);
	}
	/* reviewed */
	[CCode (cprefix = "symbols_")]
	namespace Symbols {
		public unowned string	get_context_separator (FiletypeID ft_id);
	}
	/* reviewed */
	[CCode (cprefix = "templates_")]
	namespace Templates {
		public string	get_template_fileheader (int filetype_idx, string fname);
	}
	/* reviewed */
	[CCode (cprefix = "TM", lower_case_cprefix = "tm_")]
	namespace TagManager {
		/* TODO: add TMFileEntry? not sure it's useful */
		[Compact]
		[CCode (free_function = "tm_work_object_free")]
		public class WorkObject {
			public uint				type;
			public string			file_name;
			public string			short_name;
			public WorkObject?		parent;
			public time_t			analyze_time;
			public GLib.PtrArray	tags_array;
		}
		[Compact]
		public class Workspace : WorkObject {
			public GLib.PtrArray	global_tags;
			public GLib.PtrArray	work_objects;
			
			public static bool		add_object (WorkObject work_object);
			public static bool		remove_object (WorkObject w, bool do_free, bool update);
		}
		[Compact]
		public class SourceFile : WorkObject {
			public LangType		lang;
			public bool			inactive;

			public SourceFile (string? file_name, bool update, string? name = null);
			public bool			update (bool force = true, bool recurse = true, bool update_parent = true);
		}
		[Compact]
		public class Project : WorkObject {
			public string			dir;
			[CCode (array_length = false, array_null_terminated = true)]
			public string[]			sources;
			[CCode (array_length = false, array_null_terminated = true)]
			public string[]			ignore;
			public GLib.PtrArray	file_list;
		}
		[SimpleType]
		[IntegerType]
		[CCode (cname = "langType")]
		public struct LangType : int {
		}
		[Flags]
		[CCode (has_type_id = false)]
		public enum TagType {
			[CCode (cname = "tm_tag_undef_t")]
			UNDEF,
			[CCode (cname = "tm_tag_class_t")]
			CLASS,
			[CCode (cname = "tm_tag_enum_t")]
			ENUM,
			[CCode (cname = "tm_tag_enumerator_t")]
			ENUMERATOR,
			[CCode (cname = "tm_tag_field_t")]
			FIELD,
			[CCode (cname = "tm_tag_function_t")]
			FUNCTION,
			[CCode (cname = "tm_tag_interface_t")]
			INTERFACE,
			[CCode (cname = "tm_tag_member_t")]
			MEMBER,
			[CCode (cname = "tm_tag_method_t")]
			METHOD,
			[CCode (cname = "tm_tag_namespace_t")]
			NAMESPACE,
			[CCode (cname = "tm_tag_package_t")]
			PACKAGE,
			[CCode (cname = "tm_tag_prototype_t")]
			PROTOTYPE,
			[CCode (cname = "tm_tag_struct_t")]
			STRUCT,
			[CCode (cname = "tm_tag_typedef_t")]
			TYPEDEF,
			[CCode (cname = "tm_tag_union_t")]
			UNION,
			[CCode (cname = "tm_tag_variable_t")]
			VARIABLE,
			[CCode (cname = "tm_tag_externvar_t")]
			EXTERNVAR,
			[CCode (cname = "tm_tag_macro_t")]
			MACRO,
			[CCode (cname = "tm_tag_macro_with_arg_t")]
			MACRO_WITH_ARG,
			[CCode (cname = "tm_tag_file_t")]
			FILE,
			[CCode (cname = "tm_tag_other_t")]
			OTHER,
			[CCode (cname = "tm_tag_max_t")]
			MAX
		}
		[Flags]
		[CCode (has_type_id = false)]
		public enum TagAttrType {
			[CCode (cname = "tm_tag_attr_none_t")]
			NONE,
			[CCode (cname = "tm_tag_attr_name_t")]
			NAME,
			[CCode (cname = "tm_tag_attr_type_t")]
			TYPE,
			[CCode (cname = "tm_tag_attr_file_t")]
			FILE,
			[CCode (cname = "tm_tag_attr_line_t")]
			LINE,
			[CCode (cname = "tm_tag_attr_pos_t")]
			POS,
			[CCode (cname = "tm_tag_attr_scope_t")]
			SCOPE,
			[CCode (cname = "tm_tag_attr_inheritance_t")]
			INHERITANCE,
			[CCode (cname = "tm_tag_attr_arglist_t")]
			ARGLIST,
			[CCode (cname = "tm_tag_attr_local_t")]
			LOCAL,
			[CCode (cname = "tm_tag_attr_time_t")]
			TIME,
			[CCode (cname = "tm_tag_attr_vartype_t")]
			VARTYPE,
			[CCode (cname = "tm_tag_attr_access_t")]
			ACCESS,
			[CCode (cname = "tm_tag_attr_impl_t")]
			IMPL,
			[CCode (cname = "tm_tag_attr_lang_t")]
			LANG,
			[CCode (cname = "tm_tag_attr_inactive_t")]
			INACTIVE,
			[CCode (cname = "tm_tag_attr_pointer_t")]
			POINTER,
			[CCode (cname = "tm_tag_attr_max_t")]
			MAX
		}
		[CCode (cprefix = "TAG_ACCESS_")]
		namespace TagAccess {
			char PUBLIC;
			char PROTECTED;
			char PRIVATE;
			char FRIEND;
			char DEFAULT;
			char UNKNOWN;
		}
		[CCode (cprefix = "TAG_IMPL_")]
		namespace TagImplementation {
			char VIRTUAL;
			char UNKNOWN;
		}
		/* this is a dummy proxy structure because Vala doesn't support inline anonymous structs */
		[CCode (cname = "__GeanyValaPluginTMTagAttributesEntry")]
		public struct TagAttributesEntry {
			public SourceFile	file;
			public ulong		line;
			public bool			local;
			[CCode (cname = "pointerOrder")]
			public uint			pointer_order;
			public string?		arglist;
			public string?		scope;
			public string?		inheritance;
			public string?		var_type;
			public char			access;
			public char			impl;
		}
		/* this is a dummy proxy structure because Vala doesn't support inline anonymous structs */
		[CCode (cname = "__GeanyValaPluginTMTagAttributesFile")]
		public struct TagAttributesFile {
			public time_t		timestamp;
			public LangType		lang;
			public bool			inactive;
		}
		/* this is a dummy proxy structure because Vala doesn't support inline anonymous structs */
		[CCode (cname = "__GeanyValaPluginTMTagAttributes")]
		public struct TagAttributes {
			public TagAttributesEntry	entry;
			public TagAttributesFile	file;
		}
		[Compact]
		[CCode (cname = "TMTag", cprefix = "tm_tag_")]
		public class Tag {
			public string			name;
			public TagType			type;
			[CCode (cname = "atts")]
			public TagAttributes	attributes;
		}
		/* this is a dummy proxy structure because Vala doesn't support inline anonymous structs */
		[CCode (cname = "__GeanyValaPluginTMSymbolInfo")]
		public struct SymbolInfo {
			public GLib.PtrArray	children;
			public Tag				equiv;
		}
		[Compact]
		[CCode (cname = "TMSymbol", cprefix = "tm_symbol_tree_" /*, free_function = "tm_symbol_tree_free"*/)]
		public class Symbol {
			public Tag			tag;
			public Symbol?		parent;
			public SymbolInfo	info;
		}
		
		public string		get_real_path (string file_name);
	}
	/* reviewed */
	[CCode (cprefix = "ui_", lower_case_cprefix = "ui_")]
	namespace Ui {
		[CCode (type = "GtkWidget *")]
		public Gtk.Button			button_new_with_image (string stock_id, string text);
		[CCode (type = "GtkWidget *")]
		public Gtk.ImageMenuItem	image_menu_item_new(string stock_id, string label);
		[CCode (type = "GtkWidget *")]
		public Gtk.HBox				path_box_new(string? title, Gtk.FileChooserAction action, Gtk.Entry entry);
		[CCode (type = "GtkWidget *")]
		public Gtk.VBox				dialog_vbox_new(Gtk.Dialog dialog);
		[CCode (type = "GtkWidget *")]
		public Gtk.Frame			frame_new_with_alignment(string label_text, out Gtk.Alignment alignment);
		public void					hookup_widget (Gtk.Widget owner, Gtk.Widget widget, string name);
		public void					add_document_sensitive (Gtk.Widget widget);
		public void					combo_box_add_to_history (Gtk.ComboBoxEntry combo_entry, string? text,
															  int history_len = 0);
		public void					entry_add_clear_icon (Gtk.Entry entry);
		public int					get_gtk_settings_integer (string property_name, int default_value);
		public bool					is_keyval_enter_or_return (uint keyval);
		public unowned Gtk.Widget?	lookup_widget (Gtk.Widget widget, string widget_name);
		public void					menu_add_document_items (Gtk.Menu menu, Document? active, GLib.Callback cb);
		public void					progress_bar_start (string? text);
		public void					progress_bar_stop ();
		[PrintfFormat]
		public void					set_statusbar (bool log, string format, ...);
		public void					table_add_row (Gtk.Table table, int row, ...);
		public void					widget_modify_font_from_string (Gtk.Widget widget, string str);
		public void					widget_set_tooltip_text (Gtk.Widget widget, string text);
	}
	/* reviewed */
	[CCode (cprefix = "utils_", lower_case_cprefix = "utils_")]
	namespace Utils {
		[CCode (array_length = false, array_null_terminated = true)]
		public string[]				copy_environment ([CCode (array_length = false, array_null_terminated = true)]
													  string[]? exclude_vars, string first_varname, ...);
		public string?				find_open_xml_tag (string sel, int size);
		public string				get_date_time (string format, time_t? time_to_use = null);
		public GLib.SList<string>?	get_file_list (string path, out uint length = null) throws GLib.Error;
		public GLib.SList<string>?	get_file_list_full (string path, bool full_path = false, bool sort = false)
			throws GLib.Error;
		public string				get_locale_from_utf8 (string utf8_text);
		public bool					get_setting_boolean (GLib.KeyFile config, string section,
														 string key, bool default_value);
		public int					get_setting_integer (GLib.KeyFile config, string section,
														 string key, int default_value);
		public string				get_setting_string (GLib.KeyFile config, string section,
														string key, string default_value);
		public string				get_utf8_from_locale (string locale_text);
		public int					mkdir (string path, bool create_parent_dirs = true);
		public void					open_browser (string uri);
		public string				remove_ext_from_filename (string filename);
		public bool					spawn_async (string? dir,
												 [CCode (array_length = false, array_null_terminated = true)] string[] argv,
												 [CCode (array_length = false, array_null_terminated = true)] string[]? env = null,
												 GLib.SpawnFlags flags = 0, GLib.SpawnChildSetupFunc? child_setup = null,
												 out GLib.Pid child_pid = null) throws GLib.Error;
		public bool					spawn_sync (string? dir,
												[CCode (array_length = false, array_null_terminated = true)] string[] argv,
												[CCode (array_length = false, array_null_terminated = true)] string[]? env = null,
												GLib.SpawnFlags flags = 0, GLib.SpawnChildSetupFunc? child_setup = null,
												out string std_out = null, out string std_err = null,
												out int exit_status = null) throws GLib.Error;
		public int					str_casecmp (string? s1, string? s2);
		public bool 				str_equal(string? a, string? b);
		public string				str_middle_truncate (string str, uint truncate_length);
		/* TODO: test if this works (edits the string in-place) */
		public unowned string		str_remove_chars (string str, string chars);
		public uint					string_replace_all (GLib.StringBuilder haystack, string needle, string replace);
		public uint					string_replace_first (GLib.StringBuilder haystack, string needle, string replace);
		public int					write_file (string filename, string text);
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_INDENT_TYPE_", has_type_id = false)]
	public enum IndentType {
		SPACES,
		TABS,
		BOTH
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_AUTOINDENT_", has_type_id = false)]
	public enum AutoIndent {
		NONE,
		BASIC,
		CURRENTCHARS,
		MATCHBRACES
	}
	/* reviewed */
	[CCode (cprefix = "GEANY_VIRTUAL_SPACE_", has_type_id = false)]
	public enum VirtualSpace {
		DISABLED,
		SELECTION,
		ALWAYS
	}
}

[CCode (cheader_filename="Scintilla.h", cprefix="SC", has_type_id = false)]
	namespace Scintilla {
		/* Editor.create_widget() is used to create instances */
		[CCode(cname = "ScintillaObject", cheader_filename="ScintillaWidget.h", cprefix="sci_")]
		public class Object : Gtk.Container {
			private Object() { }

			[CCode (cname = "scintilla_send_message")]
			public long				send_message (uint iMessage, ulong wParam = 0, ulong lParam = 0);
			public void				delete_marker_at_line (int line_number, int marker);
			public void				end_undo_action ();
			public void				ensure_line_is_visible (int line);
			public int				find_matching_brace (int pos);
			/* FIXME: API for TextToFind
			 * public int				find_text (int flags, TextToFind ttf); */
			public char				get_char_at (int pos);
			public int				get_col_from_position (int position);
			public string			get_contents (int len = this.get_length () + 1);
			public string			get_contents_range (int start, int end);
			public int				get_current_line ();
			public int				get_current_position ();
			public int				get_length ();
			public int				get_lexer ();
			public string			get_line (int line_num);
			public int				get_line_count ();
			public int				get_line_end_position (int line);
			public int				get_line_from_position (int position);
			public int				get_line_indentation (int line);
			public bool				get_line_is_visible (int line);
			public int				get_line_length (int line);
			public int				get_position_from_line (int line);
			public int				get_selected_text_length ();
			public string			get_selection_contents ();
			public int				get_selection_end ();
			public int				get_selection_mode ();
			public int				get_selection_start ();
			public int				get_style_at (int position);
			public int				get_tab_width ();
			public void				goto_line (int line, bool unfold = true);
			public bool				has_selection ();
			public void				indicator_clear (int start, int end);
			public void				indicator_set (int indic);
			public void 			insert_text (int pos, string text);
			public bool				is_marker_set_at_line (int line, int marker);
			public void				replace_sel (string text);
			public int				replace_target (string text, bool regex = false);
			public void				scroll_caret ();
			public void				set_current_position (int position, bool scroll_to_caret = true);
			public void				set_font (int style, string font, int size);
			public void				set_line_indentation (int line, int indent);
			public void				set_marker_at_line (int line_number, int marker);
			public void				set_selection_end (int position);
			public void				set_selection_mode (int mode);
			public void				set_selection_start (int position);
			public void				set_target_end (int end);
			public void				set_target_start (int start);
			public void				set_text (string text);
			public void				start_undo_action ();
		}
		[CCode (cname = "Sci_NotifyHeader")]
		public class NotifyHeader {
			public void				*hwndFrom;
			public ulong			idFrom;
			public uint				code;
		}
		[CCode (cname = "SCNotification")]
		public class Notification : NotifyHeader {
			int						position;
			int						ch;
			int						modifiers;
			int						modificationType;
			string					text;
			int						length;
			int						linesAdded;
			int						message;
			long					wParam;
			long					lParam;
			int						line;
			int						foldLevelNow;
			int						foldLevelPrev;
			int						margin;
			int						listType;
			int						x;
			int						y;
			int						token;
			int						annotationLinesAdded;
			int						updated;
		}
		/* generated from Scintilla.h using search&replace with:
		 * regex: #define ([0-9A-Za-z_]+)\ ([0-9xXA-F\-]+)$
		 * replace with: [CCode (cname="\1")]\npublic static int \1;
		 */
		[CCode (cname="INVALID_POSITION")]
		public static int INVALID_POSITION;
		[CCode (cname="SCI_START")]
		public static int SCI_START;
		[CCode (cname="SCI_OPTIONAL_START")]
		public static int SCI_OPTIONAL_START;
		[CCode (cname="SCI_LEXER_START")]
		public static int SCI_LEXER_START;
		[CCode (cname="SCI_ADDTEXT")]
		public static int SCI_ADDTEXT;
		[CCode (cname="SCI_ADDSTYLEDTEXT")]
		public static int SCI_ADDSTYLEDTEXT;
		[CCode (cname="SCI_INSERTTEXT")]
		public static int SCI_INSERTTEXT;
		[CCode (cname="SCI_CHANGEINSERTION")]
		public static int SCI_CHANGEINSERTION;
		[CCode (cname="SCI_CLEARALL")]
		public static int SCI_CLEARALL;
		[CCode (cname="SCI_DELETERANGE")]
		public static int SCI_DELETERANGE;
		[CCode (cname="SCI_CLEARDOCUMENTSTYLE")]
		public static int SCI_CLEARDOCUMENTSTYLE;
		[CCode (cname="SCI_GETLENGTH")]
		public static int SCI_GETLENGTH;
		[CCode (cname="SCI_GETCHARAT")]
		public static int SCI_GETCHARAT;
		[CCode (cname="SCI_GETCURRENTPOS")]
		public static int SCI_GETCURRENTPOS;
		[CCode (cname="SCI_GETANCHOR")]
		public static int SCI_GETANCHOR;
		[CCode (cname="SCI_GETSTYLEAT")]
		public static int SCI_GETSTYLEAT;
		[CCode (cname="SCI_REDO")]
		public static int SCI_REDO;
		[CCode (cname="SCI_SETUNDOCOLLECTION")]
		public static int SCI_SETUNDOCOLLECTION;
		[CCode (cname="SCI_SELECTALL")]
		public static int SCI_SELECTALL;
		[CCode (cname="SCI_SETSAVEPOINT")]
		public static int SCI_SETSAVEPOINT;
		[CCode (cname="SCI_GETSTYLEDTEXT")]
		public static int SCI_GETSTYLEDTEXT;
		[CCode (cname="SCI_CANREDO")]
		public static int SCI_CANREDO;
		[CCode (cname="SCI_MARKERLINEFROMHANDLE")]
		public static int SCI_MARKERLINEFROMHANDLE;
		[CCode (cname="SCI_MARKERDELETEHANDLE")]
		public static int SCI_MARKERDELETEHANDLE;
		[CCode (cname="SCI_GETUNDOCOLLECTION")]
		public static int SCI_GETUNDOCOLLECTION;
		[CCode (cname="SCWS_INVISIBLE")]
		public static int SCWS_INVISIBLE;
		[CCode (cname="SCWS_VISIBLEALWAYS")]
		public static int SCWS_VISIBLEALWAYS;
		[CCode (cname="SCWS_VISIBLEAFTERINDENT")]
		public static int SCWS_VISIBLEAFTERINDENT;
		[CCode (cname="SCI_GETVIEWWS")]
		public static int SCI_GETVIEWWS;
		[CCode (cname="SCI_SETVIEWWS")]
		public static int SCI_SETVIEWWS;
		[CCode (cname="SCI_POSITIONFROMPOINT")]
		public static int SCI_POSITIONFROMPOINT;
		[CCode (cname="SCI_POSITIONFROMPOINTCLOSE")]
		public static int SCI_POSITIONFROMPOINTCLOSE;
		[CCode (cname="SCI_GOTOLINE")]
		public static int SCI_GOTOLINE;
		[CCode (cname="SCI_GOTOPOS")]
		public static int SCI_GOTOPOS;
		[CCode (cname="SCI_SETANCHOR")]
		public static int SCI_SETANCHOR;
		[CCode (cname="SCI_GETCURLINE")]
		public static int SCI_GETCURLINE;
		[CCode (cname="SCI_GETENDSTYLED")]
		public static int SCI_GETENDSTYLED;
		[CCode (cname="SC_EOL_CRLF")]
		public static int SC_EOL_CRLF;
		[CCode (cname="SC_EOL_CR")]
		public static int SC_EOL_CR;
		[CCode (cname="SC_EOL_LF")]
		public static int SC_EOL_LF;
		[CCode (cname="SCI_CONVERTEOLS")]
		public static int SCI_CONVERTEOLS;
		[CCode (cname="SCI_GETEOLMODE")]
		public static int SCI_GETEOLMODE;
		[CCode (cname="SCI_SETEOLMODE")]
		public static int SCI_SETEOLMODE;
		[CCode (cname="SCI_STARTSTYLING")]
		public static int SCI_STARTSTYLING;
		[CCode (cname="SCI_SETSTYLING")]
		public static int SCI_SETSTYLING;
		[CCode (cname="SCI_GETBUFFEREDDRAW")]
		public static int SCI_GETBUFFEREDDRAW;
		[CCode (cname="SCI_SETBUFFEREDDRAW")]
		public static int SCI_SETBUFFEREDDRAW;
		[CCode (cname="SCI_SETTABWIDTH")]
		public static int SCI_SETTABWIDTH;
		[CCode (cname="SCI_GETTABWIDTH")]
		public static int SCI_GETTABWIDTH;
		[CCode (cname="SC_CP_UTF8")]
		public static int SC_CP_UTF8;
		[CCode (cname="SCI_SETCODEPAGE")]
		public static int SCI_SETCODEPAGE;
		[CCode (cname="MARKER_MAX")]
		public static int MARKER_MAX;
		[CCode (cname="SC_MARK_CIRCLE")]
		public static int SC_MARK_CIRCLE;
		[CCode (cname="SC_MARK_ROUNDRECT")]
		public static int SC_MARK_ROUNDRECT;
		[CCode (cname="SC_MARK_ARROW")]
		public static int SC_MARK_ARROW;
		[CCode (cname="SC_MARK_SMALLRECT")]
		public static int SC_MARK_SMALLRECT;
		[CCode (cname="SC_MARK_SHORTARROW")]
		public static int SC_MARK_SHORTARROW;
		[CCode (cname="SC_MARK_EMPTY")]
		public static int SC_MARK_EMPTY;
		[CCode (cname="SC_MARK_ARROWDOWN")]
		public static int SC_MARK_ARROWDOWN;
		[CCode (cname="SC_MARK_MINUS")]
		public static int SC_MARK_MINUS;
		[CCode (cname="SC_MARK_PLUS")]
		public static int SC_MARK_PLUS;
		[CCode (cname="SC_MARK_VLINE")]
		public static int SC_MARK_VLINE;
		[CCode (cname="SC_MARK_LCORNER")]
		public static int SC_MARK_LCORNER;
		[CCode (cname="SC_MARK_TCORNER")]
		public static int SC_MARK_TCORNER;
		[CCode (cname="SC_MARK_BOXPLUS")]
		public static int SC_MARK_BOXPLUS;
		[CCode (cname="SC_MARK_BOXPLUSCONNECTED")]
		public static int SC_MARK_BOXPLUSCONNECTED;
		[CCode (cname="SC_MARK_BOXMINUS")]
		public static int SC_MARK_BOXMINUS;
		[CCode (cname="SC_MARK_BOXMINUSCONNECTED")]
		public static int SC_MARK_BOXMINUSCONNECTED;
		[CCode (cname="SC_MARK_LCORNERCURVE")]
		public static int SC_MARK_LCORNERCURVE;
		[CCode (cname="SC_MARK_TCORNERCURVE")]
		public static int SC_MARK_TCORNERCURVE;
		[CCode (cname="SC_MARK_CIRCLEPLUS")]
		public static int SC_MARK_CIRCLEPLUS;
		[CCode (cname="SC_MARK_CIRCLEPLUSCONNECTED")]
		public static int SC_MARK_CIRCLEPLUSCONNECTED;
		[CCode (cname="SC_MARK_CIRCLEMINUS")]
		public static int SC_MARK_CIRCLEMINUS;
		[CCode (cname="SC_MARK_CIRCLEMINUSCONNECTED")]
		public static int SC_MARK_CIRCLEMINUSCONNECTED;
		[CCode (cname="SC_MARK_BACKGROUND")]
		public static int SC_MARK_BACKGROUND;
		[CCode (cname="SC_MARK_DOTDOTDOT")]
		public static int SC_MARK_DOTDOTDOT;
		[CCode (cname="SC_MARK_ARROWS")]
		public static int SC_MARK_ARROWS;
		[CCode (cname="SC_MARK_PIXMAP")]
		public static int SC_MARK_PIXMAP;
		[CCode (cname="SC_MARK_FULLRECT")]
		public static int SC_MARK_FULLRECT;
		[CCode (cname="SC_MARK_LEFTRECT")]
		public static int SC_MARK_LEFTRECT;
		[CCode (cname="SC_MARK_AVAILABLE")]
		public static int SC_MARK_AVAILABLE;
		[CCode (cname="SC_MARK_UNDERLINE")]
		public static int SC_MARK_UNDERLINE;
		[CCode (cname="SC_MARK_RGBAIMAGE")]
		public static int SC_MARK_RGBAIMAGE;
		[CCode (cname="SC_MARK_BOOKMARK")]
		public static int SC_MARK_BOOKMARK;
		[CCode (cname="SC_MARK_CHARACTER")]
		public static int SC_MARK_CHARACTER;
		[CCode (cname="SC_MARKNUM_FOLDEREND")]
		public static int SC_MARKNUM_FOLDEREND;
		[CCode (cname="SC_MARKNUM_FOLDEROPENMID")]
		public static int SC_MARKNUM_FOLDEROPENMID;
		[CCode (cname="SC_MARKNUM_FOLDERMIDTAIL")]
		public static int SC_MARKNUM_FOLDERMIDTAIL;
		[CCode (cname="SC_MARKNUM_FOLDERTAIL")]
		public static int SC_MARKNUM_FOLDERTAIL;
		[CCode (cname="SC_MARKNUM_FOLDERSUB")]
		public static int SC_MARKNUM_FOLDERSUB;
		[CCode (cname="SC_MARKNUM_FOLDER")]
		public static int SC_MARKNUM_FOLDER;
		[CCode (cname="SC_MARKNUM_FOLDEROPEN")]
		public static int SC_MARKNUM_FOLDEROPEN;
		[CCode (cname="SC_MASK_FOLDERS")]
		public static int SC_MASK_FOLDERS;
		[CCode (cname="SCI_MARKERDEFINE")]
		public static int SCI_MARKERDEFINE;
		[CCode (cname="SCI_MARKERSETFORE")]
		public static int SCI_MARKERSETFORE;
		[CCode (cname="SCI_MARKERSETBACK")]
		public static int SCI_MARKERSETBACK;
		[CCode (cname="SCI_MARKERSETBACKSELECTED")]
		public static int SCI_MARKERSETBACKSELECTED;
		[CCode (cname="SCI_MARKERENABLEHIGHLIGHT")]
		public static int SCI_MARKERENABLEHIGHLIGHT;
		[CCode (cname="SCI_MARKERADD")]
		public static int SCI_MARKERADD;
		[CCode (cname="SCI_MARKERDELETE")]
		public static int SCI_MARKERDELETE;
		[CCode (cname="SCI_MARKERDELETEALL")]
		public static int SCI_MARKERDELETEALL;
		[CCode (cname="SCI_MARKERGET")]
		public static int SCI_MARKERGET;
		[CCode (cname="SCI_MARKERNEXT")]
		public static int SCI_MARKERNEXT;
		[CCode (cname="SCI_MARKERPREVIOUS")]
		public static int SCI_MARKERPREVIOUS;
		[CCode (cname="SCI_MARKERDEFINEPIXMAP")]
		public static int SCI_MARKERDEFINEPIXMAP;
		[CCode (cname="SCI_MARKERADDSET")]
		public static int SCI_MARKERADDSET;
		[CCode (cname="SCI_MARKERSETALPHA")]
		public static int SCI_MARKERSETALPHA;
		[CCode (cname="SC_MAX_MARGIN")]
		public static int SC_MAX_MARGIN;
		[CCode (cname="SC_MARGIN_SYMBOL")]
		public static int SC_MARGIN_SYMBOL;
		[CCode (cname="SC_MARGIN_NUMBER")]
		public static int SC_MARGIN_NUMBER;
		[CCode (cname="SC_MARGIN_BACK")]
		public static int SC_MARGIN_BACK;
		[CCode (cname="SC_MARGIN_FORE")]
		public static int SC_MARGIN_FORE;
		[CCode (cname="SC_MARGIN_TEXT")]
		public static int SC_MARGIN_TEXT;
		[CCode (cname="SC_MARGIN_RTEXT")]
		public static int SC_MARGIN_RTEXT;
		[CCode (cname="SCI_SETMARGINTYPEN")]
		public static int SCI_SETMARGINTYPEN;
		[CCode (cname="SCI_GETMARGINTYPEN")]
		public static int SCI_GETMARGINTYPEN;
		[CCode (cname="SCI_SETMARGINWIDTHN")]
		public static int SCI_SETMARGINWIDTHN;
		[CCode (cname="SCI_GETMARGINWIDTHN")]
		public static int SCI_GETMARGINWIDTHN;
		[CCode (cname="SCI_SETMARGINMASKN")]
		public static int SCI_SETMARGINMASKN;
		[CCode (cname="SCI_GETMARGINMASKN")]
		public static int SCI_GETMARGINMASKN;
		[CCode (cname="SCI_SETMARGINSENSITIVEN")]
		public static int SCI_SETMARGINSENSITIVEN;
		[CCode (cname="SCI_GETMARGINSENSITIVEN")]
		public static int SCI_GETMARGINSENSITIVEN;
		[CCode (cname="SCI_SETMARGINCURSORN")]
		public static int SCI_SETMARGINCURSORN;
		[CCode (cname="SCI_GETMARGINCURSORN")]
		public static int SCI_GETMARGINCURSORN;
		[CCode (cname="STYLE_DEFAULT")]
		public static int STYLE_DEFAULT;
		[CCode (cname="STYLE_LINENUMBER")]
		public static int STYLE_LINENUMBER;
		[CCode (cname="STYLE_BRACELIGHT")]
		public static int STYLE_BRACELIGHT;
		[CCode (cname="STYLE_BRACEBAD")]
		public static int STYLE_BRACEBAD;
		[CCode (cname="STYLE_CONTROLCHAR")]
		public static int STYLE_CONTROLCHAR;
		[CCode (cname="STYLE_INDENTGUIDE")]
		public static int STYLE_INDENTGUIDE;
		[CCode (cname="STYLE_CALLTIP")]
		public static int STYLE_CALLTIP;
		[CCode (cname="STYLE_LASTPREDEFINED")]
		public static int STYLE_LASTPREDEFINED;
		[CCode (cname="STYLE_MAX")]
		public static int STYLE_MAX;
		[CCode (cname="SC_CHARSET_ANSI")]
		public static int SC_CHARSET_ANSI;
		[CCode (cname="SC_CHARSET_DEFAULT")]
		public static int SC_CHARSET_DEFAULT;
		[CCode (cname="SC_CHARSET_BALTIC")]
		public static int SC_CHARSET_BALTIC;
		[CCode (cname="SC_CHARSET_CHINESEBIG5")]
		public static int SC_CHARSET_CHINESEBIG5;
		[CCode (cname="SC_CHARSET_EASTEUROPE")]
		public static int SC_CHARSET_EASTEUROPE;
		[CCode (cname="SC_CHARSET_GB2312")]
		public static int SC_CHARSET_GB2312;
		[CCode (cname="SC_CHARSET_GREEK")]
		public static int SC_CHARSET_GREEK;
		[CCode (cname="SC_CHARSET_HANGUL")]
		public static int SC_CHARSET_HANGUL;
		[CCode (cname="SC_CHARSET_MAC")]
		public static int SC_CHARSET_MAC;
		[CCode (cname="SC_CHARSET_OEM")]
		public static int SC_CHARSET_OEM;
		[CCode (cname="SC_CHARSET_RUSSIAN")]
		public static int SC_CHARSET_RUSSIAN;
		[CCode (cname="SC_CHARSET_CYRILLIC")]
		public static int SC_CHARSET_CYRILLIC;
		[CCode (cname="SC_CHARSET_SHIFTJIS")]
		public static int SC_CHARSET_SHIFTJIS;
		[CCode (cname="SC_CHARSET_SYMBOL")]
		public static int SC_CHARSET_SYMBOL;
		[CCode (cname="SC_CHARSET_TURKISH")]
		public static int SC_CHARSET_TURKISH;
		[CCode (cname="SC_CHARSET_JOHAB")]
		public static int SC_CHARSET_JOHAB;
		[CCode (cname="SC_CHARSET_HEBREW")]
		public static int SC_CHARSET_HEBREW;
		[CCode (cname="SC_CHARSET_ARABIC")]
		public static int SC_CHARSET_ARABIC;
		[CCode (cname="SC_CHARSET_VIETNAMESE")]
		public static int SC_CHARSET_VIETNAMESE;
		[CCode (cname="SC_CHARSET_THAI")]
		public static int SC_CHARSET_THAI;
		[CCode (cname="SC_CHARSET_8859_15")]
		public static int SC_CHARSET_8859_15;
		[CCode (cname="SCI_STYLECLEARALL")]
		public static int SCI_STYLECLEARALL;
		[CCode (cname="SCI_STYLESETFORE")]
		public static int SCI_STYLESETFORE;
		[CCode (cname="SCI_STYLESETBACK")]
		public static int SCI_STYLESETBACK;
		[CCode (cname="SCI_STYLESETBOLD")]
		public static int SCI_STYLESETBOLD;
		[CCode (cname="SCI_STYLESETITALIC")]
		public static int SCI_STYLESETITALIC;
		[CCode (cname="SCI_STYLESETSIZE")]
		public static int SCI_STYLESETSIZE;
		[CCode (cname="SCI_STYLESETFONT")]
		public static int SCI_STYLESETFONT;
		[CCode (cname="SCI_STYLESETEOLFILLED")]
		public static int SCI_STYLESETEOLFILLED;
		[CCode (cname="SCI_STYLERESETDEFAULT")]
		public static int SCI_STYLERESETDEFAULT;
		[CCode (cname="SCI_STYLESETUNDERLINE")]
		public static int SCI_STYLESETUNDERLINE;
		[CCode (cname="SC_CASE_MIXED")]
		public static int SC_CASE_MIXED;
		[CCode (cname="SC_CASE_UPPER")]
		public static int SC_CASE_UPPER;
		[CCode (cname="SC_CASE_LOWER")]
		public static int SC_CASE_LOWER;
		[CCode (cname="SCI_STYLEGETFORE")]
		public static int SCI_STYLEGETFORE;
		[CCode (cname="SCI_STYLEGETBACK")]
		public static int SCI_STYLEGETBACK;
		[CCode (cname="SCI_STYLEGETBOLD")]
		public static int SCI_STYLEGETBOLD;
		[CCode (cname="SCI_STYLEGETITALIC")]
		public static int SCI_STYLEGETITALIC;
		[CCode (cname="SCI_STYLEGETSIZE")]
		public static int SCI_STYLEGETSIZE;
		[CCode (cname="SCI_STYLEGETFONT")]
		public static int SCI_STYLEGETFONT;
		[CCode (cname="SCI_STYLEGETEOLFILLED")]
		public static int SCI_STYLEGETEOLFILLED;
		[CCode (cname="SCI_STYLEGETUNDERLINE")]
		public static int SCI_STYLEGETUNDERLINE;
		[CCode (cname="SCI_STYLEGETCASE")]
		public static int SCI_STYLEGETCASE;
		[CCode (cname="SCI_STYLEGETCHARACTERSET")]
		public static int SCI_STYLEGETCHARACTERSET;
		[CCode (cname="SCI_STYLEGETVISIBLE")]
		public static int SCI_STYLEGETVISIBLE;
		[CCode (cname="SCI_STYLEGETCHANGEABLE")]
		public static int SCI_STYLEGETCHANGEABLE;
		[CCode (cname="SCI_STYLEGETHOTSPOT")]
		public static int SCI_STYLEGETHOTSPOT;
		[CCode (cname="SCI_STYLESETCASE")]
		public static int SCI_STYLESETCASE;
		[CCode (cname="SC_FONT_SIZE_MULTIPLIER")]
		public static int SC_FONT_SIZE_MULTIPLIER;
		[CCode (cname="SCI_STYLESETSIZEFRACTIONAL")]
		public static int SCI_STYLESETSIZEFRACTIONAL;
		[CCode (cname="SCI_STYLEGETSIZEFRACTIONAL")]
		public static int SCI_STYLEGETSIZEFRACTIONAL;
		[CCode (cname="SC_WEIGHT_NORMAL")]
		public static int SC_WEIGHT_NORMAL;
		[CCode (cname="SC_WEIGHT_SEMIBOLD")]
		public static int SC_WEIGHT_SEMIBOLD;
		[CCode (cname="SC_WEIGHT_BOLD")]
		public static int SC_WEIGHT_BOLD;
		[CCode (cname="SCI_STYLESETWEIGHT")]
		public static int SCI_STYLESETWEIGHT;
		[CCode (cname="SCI_STYLEGETWEIGHT")]
		public static int SCI_STYLEGETWEIGHT;
		[CCode (cname="SCI_STYLESETCHARACTERSET")]
		public static int SCI_STYLESETCHARACTERSET;
		[CCode (cname="SCI_STYLESETHOTSPOT")]
		public static int SCI_STYLESETHOTSPOT;
		[CCode (cname="SCI_SETSELFORE")]
		public static int SCI_SETSELFORE;
		[CCode (cname="SCI_SETSELBACK")]
		public static int SCI_SETSELBACK;
		[CCode (cname="SCI_GETSELALPHA")]
		public static int SCI_GETSELALPHA;
		[CCode (cname="SCI_SETSELALPHA")]
		public static int SCI_SETSELALPHA;
		[CCode (cname="SCI_GETSELEOLFILLED")]
		public static int SCI_GETSELEOLFILLED;
		[CCode (cname="SCI_SETSELEOLFILLED")]
		public static int SCI_SETSELEOLFILLED;
		[CCode (cname="SCI_SETCARETFORE")]
		public static int SCI_SETCARETFORE;
		[CCode (cname="SCI_ASSIGNCMDKEY")]
		public static int SCI_ASSIGNCMDKEY;
		[CCode (cname="SCI_CLEARCMDKEY")]
		public static int SCI_CLEARCMDKEY;
		[CCode (cname="SCI_CLEARALLCMDKEYS")]
		public static int SCI_CLEARALLCMDKEYS;
		[CCode (cname="SCI_SETSTYLINGEX")]
		public static int SCI_SETSTYLINGEX;
		[CCode (cname="SCI_STYLESETVISIBLE")]
		public static int SCI_STYLESETVISIBLE;
		[CCode (cname="SCI_GETCARETPERIOD")]
		public static int SCI_GETCARETPERIOD;
		[CCode (cname="SCI_SETCARETPERIOD")]
		public static int SCI_SETCARETPERIOD;
		[CCode (cname="SCI_SETWORDCHARS")]
		public static int SCI_SETWORDCHARS;
		[CCode (cname="SCI_GETWORDCHARS")]
		public static int SCI_GETWORDCHARS;
		[CCode (cname="SCI_BEGINUNDOACTION")]
		public static int SCI_BEGINUNDOACTION;
		[CCode (cname="SCI_ENDUNDOACTION")]
		public static int SCI_ENDUNDOACTION;
		[CCode (cname="INDIC_PLAIN")]
		public static int INDIC_PLAIN;
		[CCode (cname="INDIC_SQUIGGLE")]
		public static int INDIC_SQUIGGLE;
		[CCode (cname="INDIC_TT")]
		public static int INDIC_TT;
		[CCode (cname="INDIC_DIAGONAL")]
		public static int INDIC_DIAGONAL;
		[CCode (cname="INDIC_STRIKE")]
		public static int INDIC_STRIKE;
		[CCode (cname="INDIC_HIDDEN")]
		public static int INDIC_HIDDEN;
		[CCode (cname="INDIC_BOX")]
		public static int INDIC_BOX;
		[CCode (cname="INDIC_ROUNDBOX")]
		public static int INDIC_ROUNDBOX;
		[CCode (cname="INDIC_STRAIGHTBOX")]
		public static int INDIC_STRAIGHTBOX;
		[CCode (cname="INDIC_DASH")]
		public static int INDIC_DASH;
		[CCode (cname="INDIC_DOTS")]
		public static int INDIC_DOTS;
		[CCode (cname="INDIC_SQUIGGLELOW")]
		public static int INDIC_SQUIGGLELOW;
		[CCode (cname="INDIC_DOTBOX")]
		public static int INDIC_DOTBOX;
		[CCode (cname="INDIC_SQUIGGLEPIXMAP")]
		public static int INDIC_SQUIGGLEPIXMAP;
		[CCode (cname="INDIC_COMPOSITIONTHICK")]
		public static int INDIC_COMPOSITIONTHICK;
		[CCode (cname="INDIC_MAX")]
		public static int INDIC_MAX;
		[CCode (cname="INDIC_CONTAINER")]
		public static int INDIC_CONTAINER;
		[CCode (cname="INDIC0_MASK")]
		public static int INDIC0_MASK;
		[CCode (cname="INDIC1_MASK")]
		public static int INDIC1_MASK;
		[CCode (cname="INDIC2_MASK")]
		public static int INDIC2_MASK;
		[CCode (cname="INDICS_MASK")]
		public static int INDICS_MASK;
		[CCode (cname="SCI_INDICSETSTYLE")]
		public static int SCI_INDICSETSTYLE;
		[CCode (cname="SCI_INDICGETSTYLE")]
		public static int SCI_INDICGETSTYLE;
		[CCode (cname="SCI_INDICSETFORE")]
		public static int SCI_INDICSETFORE;
		[CCode (cname="SCI_INDICGETFORE")]
		public static int SCI_INDICGETFORE;
		[CCode (cname="SCI_INDICSETUNDER")]
		public static int SCI_INDICSETUNDER;
		[CCode (cname="SCI_INDICGETUNDER")]
		public static int SCI_INDICGETUNDER;
		[CCode (cname="SCI_SETWHITESPACEFORE")]
		public static int SCI_SETWHITESPACEFORE;
		[CCode (cname="SCI_SETWHITESPACEBACK")]
		public static int SCI_SETWHITESPACEBACK;
		[CCode (cname="SCI_SETWHITESPACESIZE")]
		public static int SCI_SETWHITESPACESIZE;
		[CCode (cname="SCI_GETWHITESPACESIZE")]
		public static int SCI_GETWHITESPACESIZE;
		[CCode (cname="SCI_SETSTYLEBITS")]
		public static int SCI_SETSTYLEBITS;
		[CCode (cname="SCI_GETSTYLEBITS")]
		public static int SCI_GETSTYLEBITS;
		[CCode (cname="SCI_SETLINESTATE")]
		public static int SCI_SETLINESTATE;
		[CCode (cname="SCI_GETLINESTATE")]
		public static int SCI_GETLINESTATE;
		[CCode (cname="SCI_GETMAXLINESTATE")]
		public static int SCI_GETMAXLINESTATE;
		[CCode (cname="SCI_GETCARETLINEVISIBLE")]
		public static int SCI_GETCARETLINEVISIBLE;
		[CCode (cname="SCI_SETCARETLINEVISIBLE")]
		public static int SCI_SETCARETLINEVISIBLE;
		[CCode (cname="SCI_GETCARETLINEBACK")]
		public static int SCI_GETCARETLINEBACK;
		[CCode (cname="SCI_SETCARETLINEBACK")]
		public static int SCI_SETCARETLINEBACK;
		[CCode (cname="SCI_STYLESETCHANGEABLE")]
		public static int SCI_STYLESETCHANGEABLE;
		[CCode (cname="SCI_AUTOCSHOW")]
		public static int SCI_AUTOCSHOW;
		[CCode (cname="SCI_AUTOCCANCEL")]
		public static int SCI_AUTOCCANCEL;
		[CCode (cname="SCI_AUTOCACTIVE")]
		public static int SCI_AUTOCACTIVE;
		[CCode (cname="SCI_AUTOCPOSSTART")]
		public static int SCI_AUTOCPOSSTART;
		[CCode (cname="SCI_AUTOCCOMPLETE")]
		public static int SCI_AUTOCCOMPLETE;
		[CCode (cname="SCI_AUTOCSTOPS")]
		public static int SCI_AUTOCSTOPS;
		[CCode (cname="SCI_AUTOCSETSEPARATOR")]
		public static int SCI_AUTOCSETSEPARATOR;
		[CCode (cname="SCI_AUTOCGETSEPARATOR")]
		public static int SCI_AUTOCGETSEPARATOR;
		[CCode (cname="SCI_AUTOCSELECT")]
		public static int SCI_AUTOCSELECT;
		[CCode (cname="SCI_AUTOCSETCANCELATSTART")]
		public static int SCI_AUTOCSETCANCELATSTART;
		[CCode (cname="SCI_AUTOCGETCANCELATSTART")]
		public static int SCI_AUTOCGETCANCELATSTART;
		[CCode (cname="SCI_AUTOCSETFILLUPS")]
		public static int SCI_AUTOCSETFILLUPS;
		[CCode (cname="SCI_AUTOCSETCHOOSESINGLE")]
		public static int SCI_AUTOCSETCHOOSESINGLE;
		[CCode (cname="SCI_AUTOCGETCHOOSESINGLE")]
		public static int SCI_AUTOCGETCHOOSESINGLE;
		[CCode (cname="SCI_AUTOCSETIGNORECASE")]
		public static int SCI_AUTOCSETIGNORECASE;
		[CCode (cname="SCI_AUTOCGETIGNORECASE")]
		public static int SCI_AUTOCGETIGNORECASE;
		[CCode (cname="SCI_USERLISTSHOW")]
		public static int SCI_USERLISTSHOW;
		[CCode (cname="SCI_AUTOCSETAUTOHIDE")]
		public static int SCI_AUTOCSETAUTOHIDE;
		[CCode (cname="SCI_AUTOCGETAUTOHIDE")]
		public static int SCI_AUTOCGETAUTOHIDE;
		[CCode (cname="SCI_AUTOCSETDROPRESTOFWORD")]
		public static int SCI_AUTOCSETDROPRESTOFWORD;
		[CCode (cname="SCI_AUTOCGETDROPRESTOFWORD")]
		public static int SCI_AUTOCGETDROPRESTOFWORD;
		[CCode (cname="SCI_REGISTERIMAGE")]
		public static int SCI_REGISTERIMAGE;
		[CCode (cname="SCI_CLEARREGISTEREDIMAGES")]
		public static int SCI_CLEARREGISTEREDIMAGES;
		[CCode (cname="SCI_AUTOCGETTYPESEPARATOR")]
		public static int SCI_AUTOCGETTYPESEPARATOR;
		[CCode (cname="SCI_AUTOCSETTYPESEPARATOR")]
		public static int SCI_AUTOCSETTYPESEPARATOR;
		[CCode (cname="SCI_AUTOCSETMAXWIDTH")]
		public static int SCI_AUTOCSETMAXWIDTH;
		[CCode (cname="SCI_AUTOCGETMAXWIDTH")]
		public static int SCI_AUTOCGETMAXWIDTH;
		[CCode (cname="SCI_AUTOCSETMAXHEIGHT")]
		public static int SCI_AUTOCSETMAXHEIGHT;
		[CCode (cname="SCI_AUTOCGETMAXHEIGHT")]
		public static int SCI_AUTOCGETMAXHEIGHT;
		[CCode (cname="SCI_SETINDENT")]
		public static int SCI_SETINDENT;
		[CCode (cname="SCI_GETINDENT")]
		public static int SCI_GETINDENT;
		[CCode (cname="SCI_SETUSETABS")]
		public static int SCI_SETUSETABS;
		[CCode (cname="SCI_GETUSETABS")]
		public static int SCI_GETUSETABS;
		[CCode (cname="SCI_SETLINEINDENTATION")]
		public static int SCI_SETLINEINDENTATION;
		[CCode (cname="SCI_GETLINEINDENTATION")]
		public static int SCI_GETLINEINDENTATION;
		[CCode (cname="SCI_GETLINEINDENTPOSITION")]
		public static int SCI_GETLINEINDENTPOSITION;
		[CCode (cname="SCI_GETCOLUMN")]
		public static int SCI_GETCOLUMN;
		[CCode (cname="SCI_COUNTCHARACTERS")]
		public static int SCI_COUNTCHARACTERS;
		[CCode (cname="SCI_SETHSCROLLBAR")]
		public static int SCI_SETHSCROLLBAR;
		[CCode (cname="SCI_GETHSCROLLBAR")]
		public static int SCI_GETHSCROLLBAR;
		[CCode (cname="SC_IV_NONE")]
		public static int SC_IV_NONE;
		[CCode (cname="SC_IV_REAL")]
		public static int SC_IV_REAL;
		[CCode (cname="SC_IV_LOOKFORWARD")]
		public static int SC_IV_LOOKFORWARD;
		[CCode (cname="SC_IV_LOOKBOTH")]
		public static int SC_IV_LOOKBOTH;
		[CCode (cname="SCI_SETINDENTATIONGUIDES")]
		public static int SCI_SETINDENTATIONGUIDES;
		[CCode (cname="SCI_GETINDENTATIONGUIDES")]
		public static int SCI_GETINDENTATIONGUIDES;
		[CCode (cname="SCI_SETHIGHLIGHTGUIDE")]
		public static int SCI_SETHIGHLIGHTGUIDE;
		[CCode (cname="SCI_GETHIGHLIGHTGUIDE")]
		public static int SCI_GETHIGHLIGHTGUIDE;
		[CCode (cname="SCI_GETLINEENDPOSITION")]
		public static int SCI_GETLINEENDPOSITION;
		[CCode (cname="SCI_GETCODEPAGE")]
		public static int SCI_GETCODEPAGE;
		[CCode (cname="SCI_GETCARETFORE")]
		public static int SCI_GETCARETFORE;
		[CCode (cname="SCI_GETREADONLY")]
		public static int SCI_GETREADONLY;
		[CCode (cname="SCI_SETCURRENTPOS")]
		public static int SCI_SETCURRENTPOS;
		[CCode (cname="SCI_SETSELECTIONSTART")]
		public static int SCI_SETSELECTIONSTART;
		[CCode (cname="SCI_GETSELECTIONSTART")]
		public static int SCI_GETSELECTIONSTART;
		[CCode (cname="SCI_SETSELECTIONEND")]
		public static int SCI_SETSELECTIONEND;
		[CCode (cname="SCI_GETSELECTIONEND")]
		public static int SCI_GETSELECTIONEND;
		[CCode (cname="SCI_SETEMPTYSELECTION")]
		public static int SCI_SETEMPTYSELECTION;
		[CCode (cname="SCI_SETPRINTMAGNIFICATION")]
		public static int SCI_SETPRINTMAGNIFICATION;
		[CCode (cname="SCI_GETPRINTMAGNIFICATION")]
		public static int SCI_GETPRINTMAGNIFICATION;
		[CCode (cname="SC_PRINT_NORMAL")]
		public static int SC_PRINT_NORMAL;
		[CCode (cname="SC_PRINT_INVERTLIGHT")]
		public static int SC_PRINT_INVERTLIGHT;
		[CCode (cname="SC_PRINT_BLACKONWHITE")]
		public static int SC_PRINT_BLACKONWHITE;
		[CCode (cname="SC_PRINT_COLOURONWHITE")]
		public static int SC_PRINT_COLOURONWHITE;
		[CCode (cname="SC_PRINT_COLOURONWHITEDEFAULTBG")]
		public static int SC_PRINT_COLOURONWHITEDEFAULTBG;
		[CCode (cname="SCI_SETPRINTCOLOURMODE")]
		public static int SCI_SETPRINTCOLOURMODE;
		[CCode (cname="SCI_GETPRINTCOLOURMODE")]
		public static int SCI_GETPRINTCOLOURMODE;
		[CCode (cname="SCFIND_WHOLEWORD")]
		public static int SCFIND_WHOLEWORD;
		[CCode (cname="SCFIND_MATCHCASE")]
		public static int SCFIND_MATCHCASE;
		[CCode (cname="SCFIND_WORDSTART")]
		public static int SCFIND_WORDSTART;
		[CCode (cname="SCFIND_REGEXP")]
		public static int SCFIND_REGEXP;
		[CCode (cname="SCFIND_POSIX")]
		public static int SCFIND_POSIX;
		[CCode (cname="SCI_FINDTEXT")]
		public static int SCI_FINDTEXT;
		[CCode (cname="SCI_FORMATRANGE")]
		public static int SCI_FORMATRANGE;
		[CCode (cname="SCI_GETFIRSTVISIBLELINE")]
		public static int SCI_GETFIRSTVISIBLELINE;
		[CCode (cname="SCI_GETLINE")]
		public static int SCI_GETLINE;
		[CCode (cname="SCI_GETLINECOUNT")]
		public static int SCI_GETLINECOUNT;
		[CCode (cname="SCI_SETMARGINLEFT")]
		public static int SCI_SETMARGINLEFT;
		[CCode (cname="SCI_GETMARGINLEFT")]
		public static int SCI_GETMARGINLEFT;
		[CCode (cname="SCI_SETMARGINRIGHT")]
		public static int SCI_SETMARGINRIGHT;
		[CCode (cname="SCI_GETMARGINRIGHT")]
		public static int SCI_GETMARGINRIGHT;
		[CCode (cname="SCI_GETMODIFY")]
		public static int SCI_GETMODIFY;
		[CCode (cname="SCI_SETSEL")]
		public static int SCI_SETSEL;
		[CCode (cname="SCI_GETSELTEXT")]
		public static int SCI_GETSELTEXT;
		[CCode (cname="SCI_GETTEXTRANGE")]
		public static int SCI_GETTEXTRANGE;
		[CCode (cname="SCI_HIDESELECTION")]
		public static int SCI_HIDESELECTION;
		[CCode (cname="SCI_POINTXFROMPOSITION")]
		public static int SCI_POINTXFROMPOSITION;
		[CCode (cname="SCI_POINTYFROMPOSITION")]
		public static int SCI_POINTYFROMPOSITION;
		[CCode (cname="SCI_LINEFROMPOSITION")]
		public static int SCI_LINEFROMPOSITION;
		[CCode (cname="SCI_POSITIONFROMLINE")]
		public static int SCI_POSITIONFROMLINE;
		[CCode (cname="SCI_LINESCROLL")]
		public static int SCI_LINESCROLL;
		[CCode (cname="SCI_SCROLLCARET")]
		public static int SCI_SCROLLCARET;
		[CCode (cname="SCI_SCROLLRANGE")]
		public static int SCI_SCROLLRANGE;
		[CCode (cname="SCI_REPLACESEL")]
		public static int SCI_REPLACESEL;
		[CCode (cname="SCI_SETREADONLY")]
		public static int SCI_SETREADONLY;
		[CCode (cname="SCI_NULL")]
		public static int SCI_NULL;
		[CCode (cname="SCI_CANPASTE")]
		public static int SCI_CANPASTE;
		[CCode (cname="SCI_CANUNDO")]
		public static int SCI_CANUNDO;
		[CCode (cname="SCI_EMPTYUNDOBUFFER")]
		public static int SCI_EMPTYUNDOBUFFER;
		[CCode (cname="SCI_UNDO")]
		public static int SCI_UNDO;
		[CCode (cname="SCI_CUT")]
		public static int SCI_CUT;
		[CCode (cname="SCI_COPY")]
		public static int SCI_COPY;
		[CCode (cname="SCI_PASTE")]
		public static int SCI_PASTE;
		[CCode (cname="SCI_CLEAR")]
		public static int SCI_CLEAR;
		[CCode (cname="SCI_SETTEXT")]
		public static int SCI_SETTEXT;
		[CCode (cname="SCI_GETTEXT")]
		public static int SCI_GETTEXT;
		[CCode (cname="SCI_GETTEXTLENGTH")]
		public static int SCI_GETTEXTLENGTH;
		[CCode (cname="SCI_GETDIRECTFUNCTION")]
		public static int SCI_GETDIRECTFUNCTION;
		[CCode (cname="SCI_GETDIRECTPOINTER")]
		public static int SCI_GETDIRECTPOINTER;
		[CCode (cname="SCI_SETOVERTYPE")]
		public static int SCI_SETOVERTYPE;
		[CCode (cname="SCI_GETOVERTYPE")]
		public static int SCI_GETOVERTYPE;
		[CCode (cname="SCI_SETCARETWIDTH")]
		public static int SCI_SETCARETWIDTH;
		[CCode (cname="SCI_GETCARETWIDTH")]
		public static int SCI_GETCARETWIDTH;
		[CCode (cname="SCI_SETTARGETSTART")]
		public static int SCI_SETTARGETSTART;
		[CCode (cname="SCI_GETTARGETSTART")]
		public static int SCI_GETTARGETSTART;
		[CCode (cname="SCI_SETTARGETEND")]
		public static int SCI_SETTARGETEND;
		[CCode (cname="SCI_GETTARGETEND")]
		public static int SCI_GETTARGETEND;
		[CCode (cname="SCI_REPLACETARGET")]
		public static int SCI_REPLACETARGET;
		[CCode (cname="SCI_REPLACETARGETRE")]
		public static int SCI_REPLACETARGETRE;
		[CCode (cname="SCI_SEARCHINTARGET")]
		public static int SCI_SEARCHINTARGET;
		[CCode (cname="SCI_SETSEARCHFLAGS")]
		public static int SCI_SETSEARCHFLAGS;
		[CCode (cname="SCI_GETSEARCHFLAGS")]
		public static int SCI_GETSEARCHFLAGS;
		[CCode (cname="SCI_CALLTIPSHOW")]
		public static int SCI_CALLTIPSHOW;
		[CCode (cname="SCI_CALLTIPCANCEL")]
		public static int SCI_CALLTIPCANCEL;
		[CCode (cname="SCI_CALLTIPACTIVE")]
		public static int SCI_CALLTIPACTIVE;
		[CCode (cname="SCI_CALLTIPPOSSTART")]
		public static int SCI_CALLTIPPOSSTART;
		[CCode (cname="SCI_CALLTIPSETPOSSTART")]
		public static int SCI_CALLTIPSETPOSSTART;
		[CCode (cname="SCI_CALLTIPSETHLT")]
		public static int SCI_CALLTIPSETHLT;
		[CCode (cname="SCI_CALLTIPSETBACK")]
		public static int SCI_CALLTIPSETBACK;
		[CCode (cname="SCI_CALLTIPSETFORE")]
		public static int SCI_CALLTIPSETFORE;
		[CCode (cname="SCI_CALLTIPSETFOREHLT")]
		public static int SCI_CALLTIPSETFOREHLT;
		[CCode (cname="SCI_CALLTIPUSESTYLE")]
		public static int SCI_CALLTIPUSESTYLE;
		[CCode (cname="SCI_CALLTIPSETPOSITION")]
		public static int SCI_CALLTIPSETPOSITION;
		[CCode (cname="SCI_VISIBLEFROMDOCLINE")]
		public static int SCI_VISIBLEFROMDOCLINE;
		[CCode (cname="SCI_DOCLINEFROMVISIBLE")]
		public static int SCI_DOCLINEFROMVISIBLE;
		[CCode (cname="SCI_WRAPCOUNT")]
		public static int SCI_WRAPCOUNT;
		[CCode (cname="SC_FOLDLEVELBASE")]
		public static int SC_FOLDLEVELBASE;
		[CCode (cname="SC_FOLDLEVELWHITEFLAG")]
		public static int SC_FOLDLEVELWHITEFLAG;
		[CCode (cname="SC_FOLDLEVELHEADERFLAG")]
		public static int SC_FOLDLEVELHEADERFLAG;
		[CCode (cname="SC_FOLDLEVELNUMBERMASK")]
		public static int SC_FOLDLEVELNUMBERMASK;
		[CCode (cname="SCI_SETFOLDLEVEL")]
		public static int SCI_SETFOLDLEVEL;
		[CCode (cname="SCI_GETFOLDLEVEL")]
		public static int SCI_GETFOLDLEVEL;
		[CCode (cname="SCI_GETLASTCHILD")]
		public static int SCI_GETLASTCHILD;
		[CCode (cname="SCI_GETFOLDPARENT")]
		public static int SCI_GETFOLDPARENT;
		[CCode (cname="SCI_SHOWLINES")]
		public static int SCI_SHOWLINES;
		[CCode (cname="SCI_HIDELINES")]
		public static int SCI_HIDELINES;
		[CCode (cname="SCI_GETLINEVISIBLE")]
		public static int SCI_GETLINEVISIBLE;
		[CCode (cname="SCI_GETALLLINESVISIBLE")]
		public static int SCI_GETALLLINESVISIBLE;
		[CCode (cname="SCI_SETFOLDEXPANDED")]
		public static int SCI_SETFOLDEXPANDED;
		[CCode (cname="SCI_GETFOLDEXPANDED")]
		public static int SCI_GETFOLDEXPANDED;
		[CCode (cname="SCI_TOGGLEFOLD")]
		public static int SCI_TOGGLEFOLD;
		[CCode (cname="SC_FOLDACTION_CONTRACT")]
		public static int SC_FOLDACTION_CONTRACT;
		[CCode (cname="SC_FOLDACTION_EXPAND")]
		public static int SC_FOLDACTION_EXPAND;
		[CCode (cname="SC_FOLDACTION_TOGGLE")]
		public static int SC_FOLDACTION_TOGGLE;
		[CCode (cname="SCI_FOLDLINE")]
		public static int SCI_FOLDLINE;
		[CCode (cname="SCI_FOLDCHILDREN")]
		public static int SCI_FOLDCHILDREN;
		[CCode (cname="SCI_EXPANDCHILDREN")]
		public static int SCI_EXPANDCHILDREN;
		[CCode (cname="SCI_FOLDALL")]
		public static int SCI_FOLDALL;
		[CCode (cname="SCI_ENSUREVISIBLE")]
		public static int SCI_ENSUREVISIBLE;
		[CCode (cname="SC_AUTOMATICFOLD_SHOW")]
		public static int SC_AUTOMATICFOLD_SHOW;
		[CCode (cname="SC_AUTOMATICFOLD_CLICK")]
		public static int SC_AUTOMATICFOLD_CLICK;
		[CCode (cname="SC_AUTOMATICFOLD_CHANGE")]
		public static int SC_AUTOMATICFOLD_CHANGE;
		[CCode (cname="SCI_SETAUTOMATICFOLD")]
		public static int SCI_SETAUTOMATICFOLD;
		[CCode (cname="SCI_GETAUTOMATICFOLD")]
		public static int SCI_GETAUTOMATICFOLD;
		[CCode (cname="SC_FOLDFLAG_LINEBEFORE_EXPANDED")]
		public static int SC_FOLDFLAG_LINEBEFORE_EXPANDED;
		[CCode (cname="SC_FOLDFLAG_LINEBEFORE_CONTRACTED")]
		public static int SC_FOLDFLAG_LINEBEFORE_CONTRACTED;
		[CCode (cname="SC_FOLDFLAG_LINEAFTER_EXPANDED")]
		public static int SC_FOLDFLAG_LINEAFTER_EXPANDED;
		[CCode (cname="SC_FOLDFLAG_LINEAFTER_CONTRACTED")]
		public static int SC_FOLDFLAG_LINEAFTER_CONTRACTED;
		[CCode (cname="SC_FOLDFLAG_LEVELNUMBERS")]
		public static int SC_FOLDFLAG_LEVELNUMBERS;
		[CCode (cname="SC_FOLDFLAG_LINESTATE")]
		public static int SC_FOLDFLAG_LINESTATE;
		[CCode (cname="SCI_SETFOLDFLAGS")]
		public static int SCI_SETFOLDFLAGS;
		[CCode (cname="SCI_ENSUREVISIBLEENFORCEPOLICY")]
		public static int SCI_ENSUREVISIBLEENFORCEPOLICY;
		[CCode (cname="SCI_SETTABINDENTS")]
		public static int SCI_SETTABINDENTS;
		[CCode (cname="SCI_GETTABINDENTS")]
		public static int SCI_GETTABINDENTS;
		[CCode (cname="SCI_SETBACKSPACEUNINDENTS")]
		public static int SCI_SETBACKSPACEUNINDENTS;
		[CCode (cname="SCI_GETBACKSPACEUNINDENTS")]
		public static int SCI_GETBACKSPACEUNINDENTS;
		[CCode (cname="SC_TIME_FOREVER")]
		public static int SC_TIME_FOREVER;
		[CCode (cname="SCI_SETMOUSEDWELLTIME")]
		public static int SCI_SETMOUSEDWELLTIME;
		[CCode (cname="SCI_GETMOUSEDWELLTIME")]
		public static int SCI_GETMOUSEDWELLTIME;
		[CCode (cname="SCI_WORDSTARTPOSITION")]
		public static int SCI_WORDSTARTPOSITION;
		[CCode (cname="SCI_WORDENDPOSITION")]
		public static int SCI_WORDENDPOSITION;
		[CCode (cname="SC_WRAP_NONE")]
		public static int SC_WRAP_NONE;
		[CCode (cname="SC_WRAP_WORD")]
		public static int SC_WRAP_WORD;
		[CCode (cname="SC_WRAP_CHAR")]
		public static int SC_WRAP_CHAR;
		[CCode (cname="SC_WRAP_WHITESPACE")]
		public static int SC_WRAP_WHITESPACE;
		[CCode (cname="SCI_SETWRAPMODE")]
		public static int SCI_SETWRAPMODE;
		[CCode (cname="SCI_GETWRAPMODE")]
		public static int SCI_GETWRAPMODE;
		[CCode (cname="SC_WRAPVISUALFLAG_NONE")]
		public static int SC_WRAPVISUALFLAG_NONE;
		[CCode (cname="SC_WRAPVISUALFLAG_END")]
		public static int SC_WRAPVISUALFLAG_END;
		[CCode (cname="SC_WRAPVISUALFLAG_START")]
		public static int SC_WRAPVISUALFLAG_START;
		[CCode (cname="SC_WRAPVISUALFLAG_MARGIN")]
		public static int SC_WRAPVISUALFLAG_MARGIN;
		[CCode (cname="SCI_SETWRAPVISUALFLAGS")]
		public static int SCI_SETWRAPVISUALFLAGS;
		[CCode (cname="SCI_GETWRAPVISUALFLAGS")]
		public static int SCI_GETWRAPVISUALFLAGS;
		[CCode (cname="SC_WRAPVISUALFLAGLOC_DEFAULT")]
		public static int SC_WRAPVISUALFLAGLOC_DEFAULT;
		[CCode (cname="SC_WRAPVISUALFLAGLOC_END_BY_TEXT")]
		public static int SC_WRAPVISUALFLAGLOC_END_BY_TEXT;
		[CCode (cname="SC_WRAPVISUALFLAGLOC_START_BY_TEXT")]
		public static int SC_WRAPVISUALFLAGLOC_START_BY_TEXT;
		[CCode (cname="SCI_SETWRAPVISUALFLAGSLOCATION")]
		public static int SCI_SETWRAPVISUALFLAGSLOCATION;
		[CCode (cname="SCI_GETWRAPVISUALFLAGSLOCATION")]
		public static int SCI_GETWRAPVISUALFLAGSLOCATION;
		[CCode (cname="SCI_SETWRAPSTARTINDENT")]
		public static int SCI_SETWRAPSTARTINDENT;
		[CCode (cname="SCI_GETWRAPSTARTINDENT")]
		public static int SCI_GETWRAPSTARTINDENT;
		[CCode (cname="SC_WRAPINDENT_FIXED")]
		public static int SC_WRAPINDENT_FIXED;
		[CCode (cname="SC_WRAPINDENT_SAME")]
		public static int SC_WRAPINDENT_SAME;
		[CCode (cname="SC_WRAPINDENT_INDENT")]
		public static int SC_WRAPINDENT_INDENT;
		[CCode (cname="SCI_SETWRAPINDENTMODE")]
		public static int SCI_SETWRAPINDENTMODE;
		[CCode (cname="SCI_GETWRAPINDENTMODE")]
		public static int SCI_GETWRAPINDENTMODE;
		[CCode (cname="SC_CACHE_NONE")]
		public static int SC_CACHE_NONE;
		[CCode (cname="SC_CACHE_CARET")]
		public static int SC_CACHE_CARET;
		[CCode (cname="SC_CACHE_PAGE")]
		public static int SC_CACHE_PAGE;
		[CCode (cname="SC_CACHE_DOCUMENT")]
		public static int SC_CACHE_DOCUMENT;
		[CCode (cname="SCI_SETLAYOUTCACHE")]
		public static int SCI_SETLAYOUTCACHE;
		[CCode (cname="SCI_GETLAYOUTCACHE")]
		public static int SCI_GETLAYOUTCACHE;
		[CCode (cname="SCI_SETSCROLLWIDTH")]
		public static int SCI_SETSCROLLWIDTH;
		[CCode (cname="SCI_GETSCROLLWIDTH")]
		public static int SCI_GETSCROLLWIDTH;
		[CCode (cname="SCI_SETSCROLLWIDTHTRACKING")]
		public static int SCI_SETSCROLLWIDTHTRACKING;
		[CCode (cname="SCI_GETSCROLLWIDTHTRACKING")]
		public static int SCI_GETSCROLLWIDTHTRACKING;
		[CCode (cname="SCI_TEXTWIDTH")]
		public static int SCI_TEXTWIDTH;
		[CCode (cname="SCI_SETENDATLASTLINE")]
		public static int SCI_SETENDATLASTLINE;
		[CCode (cname="SCI_GETENDATLASTLINE")]
		public static int SCI_GETENDATLASTLINE;
		[CCode (cname="SCI_TEXTHEIGHT")]
		public static int SCI_TEXTHEIGHT;
		[CCode (cname="SCI_SETVSCROLLBAR")]
		public static int SCI_SETVSCROLLBAR;
		[CCode (cname="SCI_GETVSCROLLBAR")]
		public static int SCI_GETVSCROLLBAR;
		[CCode (cname="SCI_APPENDTEXT")]
		public static int SCI_APPENDTEXT;
		[CCode (cname="SCI_GETTWOPHASEDRAW")]
		public static int SCI_GETTWOPHASEDRAW;
		[CCode (cname="SCI_SETTWOPHASEDRAW")]
		public static int SCI_SETTWOPHASEDRAW;
		[CCode (cname="SC_EFF_QUALITY_MASK")]
		public static int SC_EFF_QUALITY_MASK;
		[CCode (cname="SC_EFF_QUALITY_DEFAULT")]
		public static int SC_EFF_QUALITY_DEFAULT;
		[CCode (cname="SC_EFF_QUALITY_NON_ANTIALIASED")]
		public static int SC_EFF_QUALITY_NON_ANTIALIASED;
		[CCode (cname="SC_EFF_QUALITY_ANTIALIASED")]
		public static int SC_EFF_QUALITY_ANTIALIASED;
		[CCode (cname="SC_EFF_QUALITY_LCD_OPTIMIZED")]
		public static int SC_EFF_QUALITY_LCD_OPTIMIZED;
		[CCode (cname="SCI_SETFONTQUALITY")]
		public static int SCI_SETFONTQUALITY;
		[CCode (cname="SCI_GETFONTQUALITY")]
		public static int SCI_GETFONTQUALITY;
		[CCode (cname="SCI_SETFIRSTVISIBLELINE")]
		public static int SCI_SETFIRSTVISIBLELINE;
		[CCode (cname="SC_MULTIPASTE_ONCE")]
		public static int SC_MULTIPASTE_ONCE;
		[CCode (cname="SC_MULTIPASTE_EACH")]
		public static int SC_MULTIPASTE_EACH;
		[CCode (cname="SCI_SETMULTIPASTE")]
		public static int SCI_SETMULTIPASTE;
		[CCode (cname="SCI_GETMULTIPASTE")]
		public static int SCI_GETMULTIPASTE;
		[CCode (cname="SCI_GETTAG")]
		public static int SCI_GETTAG;
		[CCode (cname="SCI_TARGETFROMSELECTION")]
		public static int SCI_TARGETFROMSELECTION;
		[CCode (cname="SCI_LINESJOIN")]
		public static int SCI_LINESJOIN;
		[CCode (cname="SCI_LINESSPLIT")]
		public static int SCI_LINESSPLIT;
		[CCode (cname="SCI_SETFOLDMARGINCOLOUR")]
		public static int SCI_SETFOLDMARGINCOLOUR;
		[CCode (cname="SCI_SETFOLDMARGINHICOLOUR")]
		public static int SCI_SETFOLDMARGINHICOLOUR;
		[CCode (cname="SCI_LINEDOWN")]
		public static int SCI_LINEDOWN;
		[CCode (cname="SCI_LINEDOWNEXTEND")]
		public static int SCI_LINEDOWNEXTEND;
		[CCode (cname="SCI_LINEUP")]
		public static int SCI_LINEUP;
		[CCode (cname="SCI_LINEUPEXTEND")]
		public static int SCI_LINEUPEXTEND;
		[CCode (cname="SCI_CHARLEFT")]
		public static int SCI_CHARLEFT;
		[CCode (cname="SCI_CHARLEFTEXTEND")]
		public static int SCI_CHARLEFTEXTEND;
		[CCode (cname="SCI_CHARRIGHT")]
		public static int SCI_CHARRIGHT;
		[CCode (cname="SCI_CHARRIGHTEXTEND")]
		public static int SCI_CHARRIGHTEXTEND;
		[CCode (cname="SCI_WORDLEFT")]
		public static int SCI_WORDLEFT;
		[CCode (cname="SCI_WORDLEFTEXTEND")]
		public static int SCI_WORDLEFTEXTEND;
		[CCode (cname="SCI_WORDRIGHT")]
		public static int SCI_WORDRIGHT;
		[CCode (cname="SCI_WORDRIGHTEXTEND")]
		public static int SCI_WORDRIGHTEXTEND;
		[CCode (cname="SCI_HOME")]
		public static int SCI_HOME;
		[CCode (cname="SCI_HOMEEXTEND")]
		public static int SCI_HOMEEXTEND;
		[CCode (cname="SCI_LINEEND")]
		public static int SCI_LINEEND;
		[CCode (cname="SCI_LINEENDEXTEND")]
		public static int SCI_LINEENDEXTEND;
		[CCode (cname="SCI_DOCUMENTSTART")]
		public static int SCI_DOCUMENTSTART;
		[CCode (cname="SCI_DOCUMENTSTARTEXTEND")]
		public static int SCI_DOCUMENTSTARTEXTEND;
		[CCode (cname="SCI_DOCUMENTEND")]
		public static int SCI_DOCUMENTEND;
		[CCode (cname="SCI_DOCUMENTENDEXTEND")]
		public static int SCI_DOCUMENTENDEXTEND;
		[CCode (cname="SCI_PAGEUP")]
		public static int SCI_PAGEUP;
		[CCode (cname="SCI_PAGEUPEXTEND")]
		public static int SCI_PAGEUPEXTEND;
		[CCode (cname="SCI_PAGEDOWN")]
		public static int SCI_PAGEDOWN;
		[CCode (cname="SCI_PAGEDOWNEXTEND")]
		public static int SCI_PAGEDOWNEXTEND;
		[CCode (cname="SCI_EDITTOGGLEOVERTYPE")]
		public static int SCI_EDITTOGGLEOVERTYPE;
		[CCode (cname="SCI_CANCEL")]
		public static int SCI_CANCEL;
		[CCode (cname="SCI_DELETEBACK")]
		public static int SCI_DELETEBACK;
		[CCode (cname="SCI_TAB")]
		public static int SCI_TAB;
		[CCode (cname="SCI_BACKTAB")]
		public static int SCI_BACKTAB;
		[CCode (cname="SCI_NEWLINE")]
		public static int SCI_NEWLINE;
		[CCode (cname="SCI_FORMFEED")]
		public static int SCI_FORMFEED;
		[CCode (cname="SCI_VCHOME")]
		public static int SCI_VCHOME;
		[CCode (cname="SCI_VCHOMEEXTEND")]
		public static int SCI_VCHOMEEXTEND;
		[CCode (cname="SCI_ZOOMIN")]
		public static int SCI_ZOOMIN;
		[CCode (cname="SCI_ZOOMOUT")]
		public static int SCI_ZOOMOUT;
		[CCode (cname="SCI_DELWORDLEFT")]
		public static int SCI_DELWORDLEFT;
		[CCode (cname="SCI_DELWORDRIGHT")]
		public static int SCI_DELWORDRIGHT;
		[CCode (cname="SCI_DELWORDRIGHTEND")]
		public static int SCI_DELWORDRIGHTEND;
		[CCode (cname="SCI_LINECUT")]
		public static int SCI_LINECUT;
		[CCode (cname="SCI_LINEDELETE")]
		public static int SCI_LINEDELETE;
		[CCode (cname="SCI_LINETRANSPOSE")]
		public static int SCI_LINETRANSPOSE;
		[CCode (cname="SCI_LINEDUPLICATE")]
		public static int SCI_LINEDUPLICATE;
		[CCode (cname="SCI_LOWERCASE")]
		public static int SCI_LOWERCASE;
		[CCode (cname="SCI_UPPERCASE")]
		public static int SCI_UPPERCASE;
		[CCode (cname="SCI_LINESCROLLDOWN")]
		public static int SCI_LINESCROLLDOWN;
		[CCode (cname="SCI_LINESCROLLUP")]
		public static int SCI_LINESCROLLUP;
		[CCode (cname="SCI_DELETEBACKNOTLINE")]
		public static int SCI_DELETEBACKNOTLINE;
		[CCode (cname="SCI_HOMEDISPLAY")]
		public static int SCI_HOMEDISPLAY;
		[CCode (cname="SCI_HOMEDISPLAYEXTEND")]
		public static int SCI_HOMEDISPLAYEXTEND;
		[CCode (cname="SCI_LINEENDDISPLAY")]
		public static int SCI_LINEENDDISPLAY;
		[CCode (cname="SCI_LINEENDDISPLAYEXTEND")]
		public static int SCI_LINEENDDISPLAYEXTEND;
		[CCode (cname="SCI_HOMEWRAP")]
		public static int SCI_HOMEWRAP;
		[CCode (cname="SCI_HOMEWRAPEXTEND")]
		public static int SCI_HOMEWRAPEXTEND;
		[CCode (cname="SCI_LINEENDWRAP")]
		public static int SCI_LINEENDWRAP;
		[CCode (cname="SCI_LINEENDWRAPEXTEND")]
		public static int SCI_LINEENDWRAPEXTEND;
		[CCode (cname="SCI_VCHOMEWRAP")]
		public static int SCI_VCHOMEWRAP;
		[CCode (cname="SCI_VCHOMEWRAPEXTEND")]
		public static int SCI_VCHOMEWRAPEXTEND;
		[CCode (cname="SCI_LINECOPY")]
		public static int SCI_LINECOPY;
		[CCode (cname="SCI_MOVECARETINSIDEVIEW")]
		public static int SCI_MOVECARETINSIDEVIEW;
		[CCode (cname="SCI_LINELENGTH")]
		public static int SCI_LINELENGTH;
		[CCode (cname="SCI_BRACEHIGHLIGHT")]
		public static int SCI_BRACEHIGHLIGHT;
		[CCode (cname="SCI_BRACEHIGHLIGHTINDICATOR")]
		public static int SCI_BRACEHIGHLIGHTINDICATOR;
		[CCode (cname="SCI_BRACEBADLIGHT")]
		public static int SCI_BRACEBADLIGHT;
		[CCode (cname="SCI_BRACEBADLIGHTINDICATOR")]
		public static int SCI_BRACEBADLIGHTINDICATOR;
		[CCode (cname="SCI_BRACEMATCH")]
		public static int SCI_BRACEMATCH;
		[CCode (cname="SCI_GETVIEWEOL")]
		public static int SCI_GETVIEWEOL;
		[CCode (cname="SCI_SETVIEWEOL")]
		public static int SCI_SETVIEWEOL;
		[CCode (cname="SCI_GETDOCPOINTER")]
		public static int SCI_GETDOCPOINTER;
		[CCode (cname="SCI_SETDOCPOINTER")]
		public static int SCI_SETDOCPOINTER;
		[CCode (cname="SCI_SETMODEVENTMASK")]
		public static int SCI_SETMODEVENTMASK;
		[CCode (cname="EDGE_NONE")]
		public static int EDGE_NONE;
		[CCode (cname="EDGE_LINE")]
		public static int EDGE_LINE;
		[CCode (cname="EDGE_BACKGROUND")]
		public static int EDGE_BACKGROUND;
		[CCode (cname="SCI_GETEDGECOLUMN")]
		public static int SCI_GETEDGECOLUMN;
		[CCode (cname="SCI_SETEDGECOLUMN")]
		public static int SCI_SETEDGECOLUMN;
		[CCode (cname="SCI_GETEDGEMODE")]
		public static int SCI_GETEDGEMODE;
		[CCode (cname="SCI_SETEDGEMODE")]
		public static int SCI_SETEDGEMODE;
		[CCode (cname="SCI_GETEDGECOLOUR")]
		public static int SCI_GETEDGECOLOUR;
		[CCode (cname="SCI_SETEDGECOLOUR")]
		public static int SCI_SETEDGECOLOUR;
		[CCode (cname="SCI_SEARCHANCHOR")]
		public static int SCI_SEARCHANCHOR;
		[CCode (cname="SCI_SEARCHNEXT")]
		public static int SCI_SEARCHNEXT;
		[CCode (cname="SCI_SEARCHPREV")]
		public static int SCI_SEARCHPREV;
		[CCode (cname="SCI_LINESONSCREEN")]
		public static int SCI_LINESONSCREEN;
		[CCode (cname="SCI_USEPOPUP")]
		public static int SCI_USEPOPUP;
		[CCode (cname="SCI_SELECTIONISRECTANGLE")]
		public static int SCI_SELECTIONISRECTANGLE;
		[CCode (cname="SCI_SETZOOM")]
		public static int SCI_SETZOOM;
		[CCode (cname="SCI_GETZOOM")]
		public static int SCI_GETZOOM;
		[CCode (cname="SCI_CREATEDOCUMENT")]
		public static int SCI_CREATEDOCUMENT;
		[CCode (cname="SCI_ADDREFDOCUMENT")]
		public static int SCI_ADDREFDOCUMENT;
		[CCode (cname="SCI_RELEASEDOCUMENT")]
		public static int SCI_RELEASEDOCUMENT;
		[CCode (cname="SCI_GETMODEVENTMASK")]
		public static int SCI_GETMODEVENTMASK;
		[CCode (cname="SCI_SETFOCUS")]
		public static int SCI_SETFOCUS;
		[CCode (cname="SCI_GETFOCUS")]
		public static int SCI_GETFOCUS;
		[CCode (cname="SC_STATUS_OK")]
		public static int SC_STATUS_OK;
		[CCode (cname="SC_STATUS_FAILURE")]
		public static int SC_STATUS_FAILURE;
		[CCode (cname="SC_STATUS_BADALLOC")]
		public static int SC_STATUS_BADALLOC;
		[CCode (cname="SCI_SETSTATUS")]
		public static int SCI_SETSTATUS;
		[CCode (cname="SCI_GETSTATUS")]
		public static int SCI_GETSTATUS;
		[CCode (cname="SCI_SETMOUSEDOWNCAPTURES")]
		public static int SCI_SETMOUSEDOWNCAPTURES;
		[CCode (cname="SCI_GETMOUSEDOWNCAPTURES")]
		public static int SCI_GETMOUSEDOWNCAPTURES;
		[CCode (cname="SC_CURSORNORMAL")]
		public static int SC_CURSORNORMAL;
		[CCode (cname="SC_CURSORARROW")]
		public static int SC_CURSORARROW;
		[CCode (cname="SC_CURSORWAIT")]
		public static int SC_CURSORWAIT;
		[CCode (cname="SC_CURSORREVERSEARROW")]
		public static int SC_CURSORREVERSEARROW;
		[CCode (cname="SCI_SETCURSOR")]
		public static int SCI_SETCURSOR;
		[CCode (cname="SCI_GETCURSOR")]
		public static int SCI_GETCURSOR;
		[CCode (cname="SCI_SETCONTROLCHARSYMBOL")]
		public static int SCI_SETCONTROLCHARSYMBOL;
		[CCode (cname="SCI_GETCONTROLCHARSYMBOL")]
		public static int SCI_GETCONTROLCHARSYMBOL;
		[CCode (cname="SCI_WORDPARTLEFT")]
		public static int SCI_WORDPARTLEFT;
		[CCode (cname="SCI_WORDPARTLEFTEXTEND")]
		public static int SCI_WORDPARTLEFTEXTEND;
		[CCode (cname="SCI_WORDPARTRIGHT")]
		public static int SCI_WORDPARTRIGHT;
		[CCode (cname="SCI_WORDPARTRIGHTEXTEND")]
		public static int SCI_WORDPARTRIGHTEXTEND;
		[CCode (cname="VISIBLE_SLOP")]
		public static int VISIBLE_SLOP;
		[CCode (cname="VISIBLE_STRICT")]
		public static int VISIBLE_STRICT;
		[CCode (cname="SCI_SETVISIBLEPOLICY")]
		public static int SCI_SETVISIBLEPOLICY;
		[CCode (cname="SCI_DELLINELEFT")]
		public static int SCI_DELLINELEFT;
		[CCode (cname="SCI_DELLINERIGHT")]
		public static int SCI_DELLINERIGHT;
		[CCode (cname="SCI_SETXOFFSET")]
		public static int SCI_SETXOFFSET;
		[CCode (cname="SCI_GETXOFFSET")]
		public static int SCI_GETXOFFSET;
		[CCode (cname="SCI_CHOOSECARETX")]
		public static int SCI_CHOOSECARETX;
		[CCode (cname="SCI_GRABFOCUS")]
		public static int SCI_GRABFOCUS;
		[CCode (cname="CARET_SLOP")]
		public static int CARET_SLOP;
		[CCode (cname="CARET_STRICT")]
		public static int CARET_STRICT;
		[CCode (cname="CARET_JUMPS")]
		public static int CARET_JUMPS;
		[CCode (cname="CARET_EVEN")]
		public static int CARET_EVEN;
		[CCode (cname="SCI_SETXCARETPOLICY")]
		public static int SCI_SETXCARETPOLICY;
		[CCode (cname="SCI_SETYCARETPOLICY")]
		public static int SCI_SETYCARETPOLICY;
		[CCode (cname="SCI_SETPRINTWRAPMODE")]
		public static int SCI_SETPRINTWRAPMODE;
		[CCode (cname="SCI_GETPRINTWRAPMODE")]
		public static int SCI_GETPRINTWRAPMODE;
		[CCode (cname="SCI_SETHOTSPOTACTIVEFORE")]
		public static int SCI_SETHOTSPOTACTIVEFORE;
		[CCode (cname="SCI_GETHOTSPOTACTIVEFORE")]
		public static int SCI_GETHOTSPOTACTIVEFORE;
		[CCode (cname="SCI_SETHOTSPOTACTIVEBACK")]
		public static int SCI_SETHOTSPOTACTIVEBACK;
		[CCode (cname="SCI_GETHOTSPOTACTIVEBACK")]
		public static int SCI_GETHOTSPOTACTIVEBACK;
		[CCode (cname="SCI_SETHOTSPOTACTIVEUNDERLINE")]
		public static int SCI_SETHOTSPOTACTIVEUNDERLINE;
		[CCode (cname="SCI_GETHOTSPOTACTIVEUNDERLINE")]
		public static int SCI_GETHOTSPOTACTIVEUNDERLINE;
		[CCode (cname="SCI_SETHOTSPOTSINGLELINE")]
		public static int SCI_SETHOTSPOTSINGLELINE;
		[CCode (cname="SCI_GETHOTSPOTSINGLELINE")]
		public static int SCI_GETHOTSPOTSINGLELINE;
		[CCode (cname="SCI_PARADOWN")]
		public static int SCI_PARADOWN;
		[CCode (cname="SCI_PARADOWNEXTEND")]
		public static int SCI_PARADOWNEXTEND;
		[CCode (cname="SCI_PARAUP")]
		public static int SCI_PARAUP;
		[CCode (cname="SCI_PARAUPEXTEND")]
		public static int SCI_PARAUPEXTEND;
		[CCode (cname="SCI_POSITIONBEFORE")]
		public static int SCI_POSITIONBEFORE;
		[CCode (cname="SCI_POSITIONAFTER")]
		public static int SCI_POSITIONAFTER;
		[CCode (cname="SCI_POSITIONRELATIVE")]
		public static int SCI_POSITIONRELATIVE;
		[CCode (cname="SCI_COPYRANGE")]
		public static int SCI_COPYRANGE;
		[CCode (cname="SCI_COPYTEXT")]
		public static int SCI_COPYTEXT;
		[CCode (cname="SC_SEL_STREAM")]
		public static int SC_SEL_STREAM;
		[CCode (cname="SC_SEL_RECTANGLE")]
		public static int SC_SEL_RECTANGLE;
		[CCode (cname="SC_SEL_LINES")]
		public static int SC_SEL_LINES;
		[CCode (cname="SC_SEL_THIN")]
		public static int SC_SEL_THIN;
		[CCode (cname="SCI_SETSELECTIONMODE")]
		public static int SCI_SETSELECTIONMODE;
		[CCode (cname="SCI_GETSELECTIONMODE")]
		public static int SCI_GETSELECTIONMODE;
		[CCode (cname="SCI_GETLINESELSTARTPOSITION")]
		public static int SCI_GETLINESELSTARTPOSITION;
		[CCode (cname="SCI_GETLINESELENDPOSITION")]
		public static int SCI_GETLINESELENDPOSITION;
		[CCode (cname="SCI_LINEDOWNRECTEXTEND")]
		public static int SCI_LINEDOWNRECTEXTEND;
		[CCode (cname="SCI_LINEUPRECTEXTEND")]
		public static int SCI_LINEUPRECTEXTEND;
		[CCode (cname="SCI_CHARLEFTRECTEXTEND")]
		public static int SCI_CHARLEFTRECTEXTEND;
		[CCode (cname="SCI_CHARRIGHTRECTEXTEND")]
		public static int SCI_CHARRIGHTRECTEXTEND;
		[CCode (cname="SCI_HOMERECTEXTEND")]
		public static int SCI_HOMERECTEXTEND;
		[CCode (cname="SCI_VCHOMERECTEXTEND")]
		public static int SCI_VCHOMERECTEXTEND;
		[CCode (cname="SCI_LINEENDRECTEXTEND")]
		public static int SCI_LINEENDRECTEXTEND;
		[CCode (cname="SCI_PAGEUPRECTEXTEND")]
		public static int SCI_PAGEUPRECTEXTEND;
		[CCode (cname="SCI_PAGEDOWNRECTEXTEND")]
		public static int SCI_PAGEDOWNRECTEXTEND;
		[CCode (cname="SCI_STUTTEREDPAGEUP")]
		public static int SCI_STUTTEREDPAGEUP;
		[CCode (cname="SCI_STUTTEREDPAGEUPEXTEND")]
		public static int SCI_STUTTEREDPAGEUPEXTEND;
		[CCode (cname="SCI_STUTTEREDPAGEDOWN")]
		public static int SCI_STUTTEREDPAGEDOWN;
		[CCode (cname="SCI_STUTTEREDPAGEDOWNEXTEND")]
		public static int SCI_STUTTEREDPAGEDOWNEXTEND;
		[CCode (cname="SCI_WORDLEFTEND")]
		public static int SCI_WORDLEFTEND;
		[CCode (cname="SCI_WORDLEFTENDEXTEND")]
		public static int SCI_WORDLEFTENDEXTEND;
		[CCode (cname="SCI_WORDRIGHTEND")]
		public static int SCI_WORDRIGHTEND;
		[CCode (cname="SCI_WORDRIGHTENDEXTEND")]
		public static int SCI_WORDRIGHTENDEXTEND;
		[CCode (cname="SCI_SETWHITESPACECHARS")]
		public static int SCI_SETWHITESPACECHARS;
		[CCode (cname="SCI_GETWHITESPACECHARS")]
		public static int SCI_GETWHITESPACECHARS;
		[CCode (cname="SCI_SETPUNCTUATIONCHARS")]
		public static int SCI_SETPUNCTUATIONCHARS;
		[CCode (cname="SCI_GETPUNCTUATIONCHARS")]
		public static int SCI_GETPUNCTUATIONCHARS;
		[CCode (cname="SCI_SETCHARSDEFAULT")]
		public static int SCI_SETCHARSDEFAULT;
		[CCode (cname="SCI_AUTOCGETCURRENT")]
		public static int SCI_AUTOCGETCURRENT;
		[CCode (cname="SCI_AUTOCGETCURRENTTEXT")]
		public static int SCI_AUTOCGETCURRENTTEXT;
		[CCode (cname="SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE")]
		public static int SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE;
		[CCode (cname="SC_CASEINSENSITIVEBEHAVIOUR_IGNORECASE")]
		public static int SC_CASEINSENSITIVEBEHAVIOUR_IGNORECASE;
		[CCode (cname="SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR")]
		public static int SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR;
		[CCode (cname="SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR")]
		public static int SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR;
		[CCode (cname="SC_MULTIAUTOC_ONCE")]
		public static int SC_MULTIAUTOC_ONCE;
		[CCode (cname="SC_MULTIAUTOC_EACH")]
		public static int SC_MULTIAUTOC_EACH;
		[CCode (cname="SCI_AUTOCSETMULTI")]
		public static int SCI_AUTOCSETMULTI;
		[CCode (cname="SCI_AUTOCGETMULTI")]
		public static int SCI_AUTOCGETMULTI;
		[CCode (cname="SC_ORDER_PRESORTED")]
		public static int SC_ORDER_PRESORTED;
		[CCode (cname="SC_ORDER_PERFORMSORT")]
		public static int SC_ORDER_PERFORMSORT;
		[CCode (cname="SC_ORDER_CUSTOM")]
		public static int SC_ORDER_CUSTOM;
		[CCode (cname="SCI_AUTOCSETORDER")]
		public static int SCI_AUTOCSETORDER;
		[CCode (cname="SCI_AUTOCGETORDER")]
		public static int SCI_AUTOCGETORDER;
		[CCode (cname="SCI_ALLOCATE")]
		public static int SCI_ALLOCATE;
		[CCode (cname="SCI_TARGETASUTF8")]
		public static int SCI_TARGETASUTF8;
		[CCode (cname="SCI_SETLENGTHFORENCODE")]
		public static int SCI_SETLENGTHFORENCODE;
		[CCode (cname="SCI_ENCODEDFROMUTF8")]
		public static int SCI_ENCODEDFROMUTF8;
		[CCode (cname="SCI_FINDCOLUMN")]
		public static int SCI_FINDCOLUMN;
		[CCode (cname="SCI_GETCARETSTICKY")]
		public static int SCI_GETCARETSTICKY;
		[CCode (cname="SCI_SETCARETSTICKY")]
		public static int SCI_SETCARETSTICKY;
		[CCode (cname="SC_CARETSTICKY_OFF")]
		public static int SC_CARETSTICKY_OFF;
		[CCode (cname="SC_CARETSTICKY_ON")]
		public static int SC_CARETSTICKY_ON;
		[CCode (cname="SC_CARETSTICKY_WHITESPACE")]
		public static int SC_CARETSTICKY_WHITESPACE;
		[CCode (cname="SCI_TOGGLECARETSTICKY")]
		public static int SCI_TOGGLECARETSTICKY;
		[CCode (cname="SCI_SETPASTECONVERTENDINGS")]
		public static int SCI_SETPASTECONVERTENDINGS;
		[CCode (cname="SCI_GETPASTECONVERTENDINGS")]
		public static int SCI_GETPASTECONVERTENDINGS;
		[CCode (cname="SCI_SELECTIONDUPLICATE")]
		public static int SCI_SELECTIONDUPLICATE;
		[CCode (cname="SC_ALPHA_TRANSPARENT")]
		public static int SC_ALPHA_TRANSPARENT;
		[CCode (cname="SC_ALPHA_OPAQUE")]
		public static int SC_ALPHA_OPAQUE;
		[CCode (cname="SC_ALPHA_NOALPHA")]
		public static int SC_ALPHA_NOALPHA;
		[CCode (cname="SCI_SETCARETLINEBACKALPHA")]
		public static int SCI_SETCARETLINEBACKALPHA;
		[CCode (cname="SCI_GETCARETLINEBACKALPHA")]
		public static int SCI_GETCARETLINEBACKALPHA;
		[CCode (cname="CARETSTYLE_INVISIBLE")]
		public static int CARETSTYLE_INVISIBLE;
		[CCode (cname="CARETSTYLE_LINE")]
		public static int CARETSTYLE_LINE;
		[CCode (cname="CARETSTYLE_BLOCK")]
		public static int CARETSTYLE_BLOCK;
		[CCode (cname="SCI_SETCARETSTYLE")]
		public static int SCI_SETCARETSTYLE;
		[CCode (cname="SCI_GETCARETSTYLE")]
		public static int SCI_GETCARETSTYLE;
		[CCode (cname="SCI_SETINDICATORCURRENT")]
		public static int SCI_SETINDICATORCURRENT;
		[CCode (cname="SCI_GETINDICATORCURRENT")]
		public static int SCI_GETINDICATORCURRENT;
		[CCode (cname="SCI_SETINDICATORVALUE")]
		public static int SCI_SETINDICATORVALUE;
		[CCode (cname="SCI_GETINDICATORVALUE")]
		public static int SCI_GETINDICATORVALUE;
		[CCode (cname="SCI_INDICATORFILLRANGE")]
		public static int SCI_INDICATORFILLRANGE;
		[CCode (cname="SCI_INDICATORCLEARRANGE")]
		public static int SCI_INDICATORCLEARRANGE;
		[CCode (cname="SCI_INDICATORALLONFOR")]
		public static int SCI_INDICATORALLONFOR;
		[CCode (cname="SCI_INDICATORVALUEAT")]
		public static int SCI_INDICATORVALUEAT;
		[CCode (cname="SCI_INDICATORSTART")]
		public static int SCI_INDICATORSTART;
		[CCode (cname="SCI_INDICATOREND")]
		public static int SCI_INDICATOREND;
		[CCode (cname="SCI_SETPOSITIONCACHE")]
		public static int SCI_SETPOSITIONCACHE;
		[CCode (cname="SCI_GETPOSITIONCACHE")]
		public static int SCI_GETPOSITIONCACHE;
		[CCode (cname="SCI_COPYALLOWLINE")]
		public static int SCI_COPYALLOWLINE;
		[CCode (cname="SCI_GETCHARACTERPOINTER")]
		public static int SCI_GETCHARACTERPOINTER;
		[CCode (cname="SCI_GETRANGEPOINTER")]
		public static int SCI_GETRANGEPOINTER;
		[CCode (cname="SCI_GETGAPPOSITION")]
		public static int SCI_GETGAPPOSITION;
		[CCode (cname="SCI_SETKEYSUNICODE")]
		public static int SCI_SETKEYSUNICODE;
		[CCode (cname="SCI_GETKEYSUNICODE")]
		public static int SCI_GETKEYSUNICODE;
		[CCode (cname="SCI_INDICSETALPHA")]
		public static int SCI_INDICSETALPHA;
		[CCode (cname="SCI_INDICGETALPHA")]
		public static int SCI_INDICGETALPHA;
		[CCode (cname="SCI_INDICSETOUTLINEALPHA")]
		public static int SCI_INDICSETOUTLINEALPHA;
		[CCode (cname="SCI_INDICGETOUTLINEALPHA")]
		public static int SCI_INDICGETOUTLINEALPHA;
		[CCode (cname="SCI_SETEXTRAASCENT")]
		public static int SCI_SETEXTRAASCENT;
		[CCode (cname="SCI_GETEXTRAASCENT")]
		public static int SCI_GETEXTRAASCENT;
		[CCode (cname="SCI_SETEXTRADESCENT")]
		public static int SCI_SETEXTRADESCENT;
		[CCode (cname="SCI_GETEXTRADESCENT")]
		public static int SCI_GETEXTRADESCENT;
		[CCode (cname="SCI_MARKERSYMBOLDEFINED")]
		public static int SCI_MARKERSYMBOLDEFINED;
		[CCode (cname="SCI_MARGINSETTEXT")]
		public static int SCI_MARGINSETTEXT;
		[CCode (cname="SCI_MARGINGETTEXT")]
		public static int SCI_MARGINGETTEXT;
		[CCode (cname="SCI_MARGINSETSTYLE")]
		public static int SCI_MARGINSETSTYLE;
		[CCode (cname="SCI_MARGINGETSTYLE")]
		public static int SCI_MARGINGETSTYLE;
		[CCode (cname="SCI_MARGINSETSTYLES")]
		public static int SCI_MARGINSETSTYLES;
		[CCode (cname="SCI_MARGINGETSTYLES")]
		public static int SCI_MARGINGETSTYLES;
		[CCode (cname="SCI_MARGINTEXTCLEARALL")]
		public static int SCI_MARGINTEXTCLEARALL;
		[CCode (cname="SCI_MARGINSETSTYLEOFFSET")]
		public static int SCI_MARGINSETSTYLEOFFSET;
		[CCode (cname="SCI_MARGINGETSTYLEOFFSET")]
		public static int SCI_MARGINGETSTYLEOFFSET;
		[CCode (cname="SC_MARGINOPTION_NONE")]
		public static int SC_MARGINOPTION_NONE;
		[CCode (cname="SC_MARGINOPTION_SUBLINESELECT")]
		public static int SC_MARGINOPTION_SUBLINESELECT;
		[CCode (cname="SCI_SETMARGINOPTIONS")]
		public static int SCI_SETMARGINOPTIONS;
		[CCode (cname="SCI_GETMARGINOPTIONS")]
		public static int SCI_GETMARGINOPTIONS;
		[CCode (cname="SCI_ANNOTATIONSETTEXT")]
		public static int SCI_ANNOTATIONSETTEXT;
		[CCode (cname="SCI_ANNOTATIONGETTEXT")]
		public static int SCI_ANNOTATIONGETTEXT;
		[CCode (cname="SCI_ANNOTATIONSETSTYLE")]
		public static int SCI_ANNOTATIONSETSTYLE;
		[CCode (cname="SCI_ANNOTATIONGETSTYLE")]
		public static int SCI_ANNOTATIONGETSTYLE;
		[CCode (cname="SCI_ANNOTATIONSETSTYLES")]
		public static int SCI_ANNOTATIONSETSTYLES;
		[CCode (cname="SCI_ANNOTATIONGETSTYLES")]
		public static int SCI_ANNOTATIONGETSTYLES;
		[CCode (cname="SCI_ANNOTATIONGETLINES")]
		public static int SCI_ANNOTATIONGETLINES;
		[CCode (cname="SCI_ANNOTATIONCLEARALL")]
		public static int SCI_ANNOTATIONCLEARALL;
		[CCode (cname="ANNOTATION_HIDDEN")]
		public static int ANNOTATION_HIDDEN;
		[CCode (cname="ANNOTATION_STANDARD")]
		public static int ANNOTATION_STANDARD;
		[CCode (cname="ANNOTATION_BOXED")]
		public static int ANNOTATION_BOXED;
		[CCode (cname="SCI_ANNOTATIONSETVISIBLE")]
		public static int SCI_ANNOTATIONSETVISIBLE;
		[CCode (cname="SCI_ANNOTATIONGETVISIBLE")]
		public static int SCI_ANNOTATIONGETVISIBLE;
		[CCode (cname="SCI_ANNOTATIONSETSTYLEOFFSET")]
		public static int SCI_ANNOTATIONSETSTYLEOFFSET;
		[CCode (cname="SCI_ANNOTATIONGETSTYLEOFFSET")]
		public static int SCI_ANNOTATIONGETSTYLEOFFSET;
		[CCode (cname="SCI_RELEASEALLEXTENDEDSTYLES")]
		public static int SCI_RELEASEALLEXTENDEDSTYLES;
		[CCode (cname="SCI_ALLOCATEEXTENDEDSTYLES")]
		public static int SCI_ALLOCATEEXTENDEDSTYLES;
		[CCode (cname="UNDO_MAY_COALESCE")]
		public static int UNDO_MAY_COALESCE;
		[CCode (cname="SCI_ADDUNDOACTION")]
		public static int SCI_ADDUNDOACTION;
		[CCode (cname="SCI_CHARPOSITIONFROMPOINT")]
		public static int SCI_CHARPOSITIONFROMPOINT;
		[CCode (cname="SCI_CHARPOSITIONFROMPOINTCLOSE")]
		public static int SCI_CHARPOSITIONFROMPOINTCLOSE;
		[CCode (cname="SCI_SETMOUSESELECTIONRECTANGULARSWITCH")]
		public static int SCI_SETMOUSESELECTIONRECTANGULARSWITCH;
		[CCode (cname="SCI_GETMOUSESELECTIONRECTANGULARSWITCH")]
		public static int SCI_GETMOUSESELECTIONRECTANGULARSWITCH;
		[CCode (cname="SCI_SETMULTIPLESELECTION")]
		public static int SCI_SETMULTIPLESELECTION;
		[CCode (cname="SCI_GETMULTIPLESELECTION")]
		public static int SCI_GETMULTIPLESELECTION;
		[CCode (cname="SCI_SETADDITIONALSELECTIONTYPING")]
		public static int SCI_SETADDITIONALSELECTIONTYPING;
		[CCode (cname="SCI_GETADDITIONALSELECTIONTYPING")]
		public static int SCI_GETADDITIONALSELECTIONTYPING;
		[CCode (cname="SCI_SETADDITIONALCARETSBLINK")]
		public static int SCI_SETADDITIONALCARETSBLINK;
		[CCode (cname="SCI_GETADDITIONALCARETSBLINK")]
		public static int SCI_GETADDITIONALCARETSBLINK;
		[CCode (cname="SCI_SETADDITIONALCARETSVISIBLE")]
		public static int SCI_SETADDITIONALCARETSVISIBLE;
		[CCode (cname="SCI_GETADDITIONALCARETSVISIBLE")]
		public static int SCI_GETADDITIONALCARETSVISIBLE;
		[CCode (cname="SCI_GETSELECTIONS")]
		public static int SCI_GETSELECTIONS;
		[CCode (cname="SCI_GETSELECTIONEMPTY")]
		public static int SCI_GETSELECTIONEMPTY;
		[CCode (cname="SCI_CLEARSELECTIONS")]
		public static int SCI_CLEARSELECTIONS;
		[CCode (cname="SCI_SETSELECTION")]
		public static int SCI_SETSELECTION;
		[CCode (cname="SCI_ADDSELECTION")]
		public static int SCI_ADDSELECTION;
		[CCode (cname="SCI_DROPSELECTIONN")]
		public static int SCI_DROPSELECTIONN;
		[CCode (cname="SCI_SETMAINSELECTION")]
		public static int SCI_SETMAINSELECTION;
		[CCode (cname="SCI_GETMAINSELECTION")]
		public static int SCI_GETMAINSELECTION;
		[CCode (cname="SCI_SETSELECTIONNCARET")]
		public static int SCI_SETSELECTIONNCARET;
		[CCode (cname="SCI_GETSELECTIONNCARET")]
		public static int SCI_GETSELECTIONNCARET;
		[CCode (cname="SCI_SETSELECTIONNANCHOR")]
		public static int SCI_SETSELECTIONNANCHOR;
		[CCode (cname="SCI_GETSELECTIONNANCHOR")]
		public static int SCI_GETSELECTIONNANCHOR;
		[CCode (cname="SCI_SETSELECTIONNCARETVIRTUALSPACE")]
		public static int SCI_SETSELECTIONNCARETVIRTUALSPACE;
		[CCode (cname="SCI_GETSELECTIONNCARETVIRTUALSPACE")]
		public static int SCI_GETSELECTIONNCARETVIRTUALSPACE;
		[CCode (cname="SCI_SETSELECTIONNANCHORVIRTUALSPACE")]
		public static int SCI_SETSELECTIONNANCHORVIRTUALSPACE;
		[CCode (cname="SCI_GETSELECTIONNANCHORVIRTUALSPACE")]
		public static int SCI_GETSELECTIONNANCHORVIRTUALSPACE;
		[CCode (cname="SCI_SETSELECTIONNSTART")]
		public static int SCI_SETSELECTIONNSTART;
		[CCode (cname="SCI_GETSELECTIONNSTART")]
		public static int SCI_GETSELECTIONNSTART;
		[CCode (cname="SCI_SETSELECTIONNEND")]
		public static int SCI_SETSELECTIONNEND;
		[CCode (cname="SCI_GETSELECTIONNEND")]
		public static int SCI_GETSELECTIONNEND;
		[CCode (cname="SCI_SETRECTANGULARSELECTIONCARET")]
		public static int SCI_SETRECTANGULARSELECTIONCARET;
		[CCode (cname="SCI_GETRECTANGULARSELECTIONCARET")]
		public static int SCI_GETRECTANGULARSELECTIONCARET;
		[CCode (cname="SCI_SETRECTANGULARSELECTIONANCHOR")]
		public static int SCI_SETRECTANGULARSELECTIONANCHOR;
		[CCode (cname="SCI_GETRECTANGULARSELECTIONANCHOR")]
		public static int SCI_GETRECTANGULARSELECTIONANCHOR;
		[CCode (cname="SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE")]
		public static int SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE;
		[CCode (cname="SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE")]
		public static int SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE;
		[CCode (cname="SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE")]
		public static int SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE;
		[CCode (cname="SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE")]
		public static int SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE;
		[CCode (cname="SCVS_NONE")]
		public static int SCVS_NONE;
		[CCode (cname="SCVS_RECTANGULARSELECTION")]
		public static int SCVS_RECTANGULARSELECTION;
		[CCode (cname="SCVS_USERACCESSIBLE")]
		public static int SCVS_USERACCESSIBLE;
		[CCode (cname="SCI_SETVIRTUALSPACEOPTIONS")]
		public static int SCI_SETVIRTUALSPACEOPTIONS;
		[CCode (cname="SCI_GETVIRTUALSPACEOPTIONS")]
		public static int SCI_GETVIRTUALSPACEOPTIONS;
		[CCode (cname="SCI_SETRECTANGULARSELECTIONMODIFIER")]
		public static int SCI_SETRECTANGULARSELECTIONMODIFIER;
		[CCode (cname="SCI_GETRECTANGULARSELECTIONMODIFIER")]
		public static int SCI_GETRECTANGULARSELECTIONMODIFIER;
		[CCode (cname="SCI_SETADDITIONALSELFORE")]
		public static int SCI_SETADDITIONALSELFORE;
		[CCode (cname="SCI_SETADDITIONALSELBACK")]
		public static int SCI_SETADDITIONALSELBACK;
		[CCode (cname="SCI_SETADDITIONALSELALPHA")]
		public static int SCI_SETADDITIONALSELALPHA;
		[CCode (cname="SCI_GETADDITIONALSELALPHA")]
		public static int SCI_GETADDITIONALSELALPHA;
		[CCode (cname="SCI_SETADDITIONALCARETFORE")]
		public static int SCI_SETADDITIONALCARETFORE;
		[CCode (cname="SCI_GETADDITIONALCARETFORE")]
		public static int SCI_GETADDITIONALCARETFORE;
		[CCode (cname="SCI_ROTATESELECTION")]
		public static int SCI_ROTATESELECTION;
		[CCode (cname="SCI_SWAPMAINANCHORCARET")]
		public static int SCI_SWAPMAINANCHORCARET;
		[CCode (cname="SCI_CHANGELEXERSTATE")]
		public static int SCI_CHANGELEXERSTATE;
		[CCode (cname="SCI_CONTRACTEDFOLDNEXT")]
		public static int SCI_CONTRACTEDFOLDNEXT;
		[CCode (cname="SCI_VERTICALCENTRECARET")]
		public static int SCI_VERTICALCENTRECARET;
		[CCode (cname="SCI_MOVESELECTEDLINESUP")]
		public static int SCI_MOVESELECTEDLINESUP;
		[CCode (cname="SCI_MOVESELECTEDLINESDOWN")]
		public static int SCI_MOVESELECTEDLINESDOWN;
		[CCode (cname="SCI_SETIDENTIFIER")]
		public static int SCI_SETIDENTIFIER;
		[CCode (cname="SCI_GETIDENTIFIER")]
		public static int SCI_GETIDENTIFIER;
		[CCode (cname="SCI_RGBAIMAGESETWIDTH")]
		public static int SCI_RGBAIMAGESETWIDTH;
		[CCode (cname="SCI_RGBAIMAGESETHEIGHT")]
		public static int SCI_RGBAIMAGESETHEIGHT;
		[CCode (cname="SCI_RGBAIMAGESETSCALE")]
		public static int SCI_RGBAIMAGESETSCALE;
		[CCode (cname="SCI_MARKERDEFINERGBAIMAGE")]
		public static int SCI_MARKERDEFINERGBAIMAGE;
		[CCode (cname="SCI_REGISTERRGBAIMAGE")]
		public static int SCI_REGISTERRGBAIMAGE;
		[CCode (cname="SCI_SCROLLTOSTART")]
		public static int SCI_SCROLLTOSTART;
		[CCode (cname="SCI_SCROLLTOEND")]
		public static int SCI_SCROLLTOEND;
		[CCode (cname="SC_TECHNOLOGY_DEFAULT")]
		public static int SC_TECHNOLOGY_DEFAULT;
		[CCode (cname="SC_TECHNOLOGY_DIRECTWRITE")]
		public static int SC_TECHNOLOGY_DIRECTWRITE;
		[CCode (cname="SCI_SETTECHNOLOGY")]
		public static int SCI_SETTECHNOLOGY;
		[CCode (cname="SCI_GETTECHNOLOGY")]
		public static int SCI_GETTECHNOLOGY;
		[CCode (cname="SCI_CREATELOADER")]
		public static int SCI_CREATELOADER;
		[CCode (cname="SCI_FINDINDICATORSHOW")]
		public static int SCI_FINDINDICATORSHOW;
		[CCode (cname="SCI_FINDINDICATORFLASH")]
		public static int SCI_FINDINDICATORFLASH;
		[CCode (cname="SCI_FINDINDICATORHIDE")]
		public static int SCI_FINDINDICATORHIDE;
		[CCode (cname="SCI_VCHOMEDISPLAY")]
		public static int SCI_VCHOMEDISPLAY;
		[CCode (cname="SCI_VCHOMEDISPLAYEXTEND")]
		public static int SCI_VCHOMEDISPLAYEXTEND;
		[CCode (cname="SCI_GETCARETLINEVISIBLEALWAYS")]
		public static int SCI_GETCARETLINEVISIBLEALWAYS;
		[CCode (cname="SCI_SETCARETLINEVISIBLEALWAYS")]
		public static int SCI_SETCARETLINEVISIBLEALWAYS;
		[CCode (cname="SC_LINE_END_TYPE_DEFAULT")]
		public static int SC_LINE_END_TYPE_DEFAULT;
		[CCode (cname="SC_LINE_END_TYPE_UNICODE")]
		public static int SC_LINE_END_TYPE_UNICODE;
		[CCode (cname="SCI_SETLINEENDTYPESALLOWED")]
		public static int SCI_SETLINEENDTYPESALLOWED;
		[CCode (cname="SCI_GETLINEENDTYPESALLOWED")]
		public static int SCI_GETLINEENDTYPESALLOWED;
		[CCode (cname="SCI_GETLINEENDTYPESACTIVE")]
		public static int SCI_GETLINEENDTYPESACTIVE;
		[CCode (cname="SCI_SETREPRESENTATION")]
		public static int SCI_SETREPRESENTATION;
		[CCode (cname="SCI_GETREPRESENTATION")]
		public static int SCI_GETREPRESENTATION;
		[CCode (cname="SCI_CLEARREPRESENTATION")]
		public static int SCI_CLEARREPRESENTATION;
		[CCode (cname="SCI_STARTRECORD")]
		public static int SCI_STARTRECORD;
		[CCode (cname="SCI_STOPRECORD")]
		public static int SCI_STOPRECORD;
		[CCode (cname="SCI_SETLEXER")]
		public static int SCI_SETLEXER;
		[CCode (cname="SCI_GETLEXER")]
		public static int SCI_GETLEXER;
		[CCode (cname="SCI_COLOURISE")]
		public static int SCI_COLOURISE;
		[CCode (cname="SCI_SETPROPERTY")]
		public static int SCI_SETPROPERTY;
		[CCode (cname="KEYWORDSET_MAX")]
		public static int KEYWORDSET_MAX;
		[CCode (cname="SCI_SETKEYWORDS")]
		public static int SCI_SETKEYWORDS;
		[CCode (cname="SCI_SETLEXERLANGUAGE")]
		public static int SCI_SETLEXERLANGUAGE;
		[CCode (cname="SCI_LOADLEXERLIBRARY")]
		public static int SCI_LOADLEXERLIBRARY;
		[CCode (cname="SCI_GETPROPERTY")]
		public static int SCI_GETPROPERTY;
		[CCode (cname="SCI_GETPROPERTYEXPANDED")]
		public static int SCI_GETPROPERTYEXPANDED;
		[CCode (cname="SCI_GETPROPERTYINT")]
		public static int SCI_GETPROPERTYINT;
		[CCode (cname="SCI_GETSTYLEBITSNEEDED")]
		public static int SCI_GETSTYLEBITSNEEDED;
		[CCode (cname="SCI_GETLEXERLANGUAGE")]
		public static int SCI_GETLEXERLANGUAGE;
		[CCode (cname="SCI_PRIVATELEXERCALL")]
		public static int SCI_PRIVATELEXERCALL;
		[CCode (cname="SCI_PROPERTYNAMES")]
		public static int SCI_PROPERTYNAMES;
		[CCode (cname="SC_TYPE_BOOLEAN")]
		public static int SC_TYPE_BOOLEAN;
		[CCode (cname="SC_TYPE_INTEGER")]
		public static int SC_TYPE_INTEGER;
		[CCode (cname="SC_TYPE_STRING")]
		public static int SC_TYPE_STRING;
		[CCode (cname="SCI_PROPERTYTYPE")]
		public static int SCI_PROPERTYTYPE;
		[CCode (cname="SCI_DESCRIBEPROPERTY")]
		public static int SCI_DESCRIBEPROPERTY;
		[CCode (cname="SCI_DESCRIBEKEYWORDSETS")]
		public static int SCI_DESCRIBEKEYWORDSETS;
		[CCode (cname="SCI_GETLINEENDTYPESSUPPORTED")]
		public static int SCI_GETLINEENDTYPESSUPPORTED;
		[CCode (cname="SCI_ALLOCATESUBSTYLES")]
		public static int SCI_ALLOCATESUBSTYLES;
		[CCode (cname="SCI_GETSUBSTYLESSTART")]
		public static int SCI_GETSUBSTYLESSTART;
		[CCode (cname="SCI_GETSUBSTYLESLENGTH")]
		public static int SCI_GETSUBSTYLESLENGTH;
		[CCode (cname="SCI_GETSTYLEFROMSUBSTYLE")]
		public static int SCI_GETSTYLEFROMSUBSTYLE;
		[CCode (cname="SCI_GETPRIMARYSTYLEFROMSTYLE")]
		public static int SCI_GETPRIMARYSTYLEFROMSTYLE;
		[CCode (cname="SCI_FREESUBSTYLES")]
		public static int SCI_FREESUBSTYLES;
		[CCode (cname="SCI_SETIDENTIFIERS")]
		public static int SCI_SETIDENTIFIERS;
		[CCode (cname="SCI_DISTANCETOSECONDARYSTYLES")]
		public static int SCI_DISTANCETOSECONDARYSTYLES;
		[CCode (cname="SCI_GETSUBSTYLEBASES")]
		public static int SCI_GETSUBSTYLEBASES;
		[CCode (cname="SC_MOD_INSERTTEXT")]
		public static int SC_MOD_INSERTTEXT;
		[CCode (cname="SC_MOD_DELETETEXT")]
		public static int SC_MOD_DELETETEXT;
		[CCode (cname="SC_MOD_CHANGESTYLE")]
		public static int SC_MOD_CHANGESTYLE;
		[CCode (cname="SC_MOD_CHANGEFOLD")]
		public static int SC_MOD_CHANGEFOLD;
		[CCode (cname="SC_PERFORMED_USER")]
		public static int SC_PERFORMED_USER;
		[CCode (cname="SC_PERFORMED_UNDO")]
		public static int SC_PERFORMED_UNDO;
		[CCode (cname="SC_PERFORMED_REDO")]
		public static int SC_PERFORMED_REDO;
		[CCode (cname="SC_MULTISTEPUNDOREDO")]
		public static int SC_MULTISTEPUNDOREDO;
		[CCode (cname="SC_LASTSTEPINUNDOREDO")]
		public static int SC_LASTSTEPINUNDOREDO;
		[CCode (cname="SC_MOD_CHANGEMARKER")]
		public static int SC_MOD_CHANGEMARKER;
		[CCode (cname="SC_MOD_BEFOREINSERT")]
		public static int SC_MOD_BEFOREINSERT;
		[CCode (cname="SC_MOD_BEFOREDELETE")]
		public static int SC_MOD_BEFOREDELETE;
		[CCode (cname="SC_MULTILINEUNDOREDO")]
		public static int SC_MULTILINEUNDOREDO;
		[CCode (cname="SC_STARTACTION")]
		public static int SC_STARTACTION;
		[CCode (cname="SC_MOD_CHANGEINDICATOR")]
		public static int SC_MOD_CHANGEINDICATOR;
		[CCode (cname="SC_MOD_CHANGELINESTATE")]
		public static int SC_MOD_CHANGELINESTATE;
		[CCode (cname="SC_MOD_CHANGEMARGIN")]
		public static int SC_MOD_CHANGEMARGIN;
		[CCode (cname="SC_MOD_CHANGEANNOTATION")]
		public static int SC_MOD_CHANGEANNOTATION;
		[CCode (cname="SC_MOD_CONTAINER")]
		public static int SC_MOD_CONTAINER;
		[CCode (cname="SC_MOD_LEXERSTATE")]
		public static int SC_MOD_LEXERSTATE;
		[CCode (cname="SC_MOD_INSERTCHECK")]
		public static int SC_MOD_INSERTCHECK;
		[CCode (cname="SC_MODEVENTMASKALL")]
		public static int SC_MODEVENTMASKALL;
		[CCode (cname="SC_UPDATE_CONTENT")]
		public static int SC_UPDATE_CONTENT;
		[CCode (cname="SC_UPDATE_SELECTION")]
		public static int SC_UPDATE_SELECTION;
		[CCode (cname="SC_UPDATE_V_SCROLL")]
		public static int SC_UPDATE_V_SCROLL;
		[CCode (cname="SC_UPDATE_H_SCROLL")]
		public static int SC_UPDATE_H_SCROLL;
		[CCode (cname="SCEN_CHANGE")]
		public static int SCEN_CHANGE;
		[CCode (cname="SCEN_SETFOCUS")]
		public static int SCEN_SETFOCUS;
		[CCode (cname="SCEN_KILLFOCUS")]
		public static int SCEN_KILLFOCUS;
		[CCode (cname="SCK_DOWN")]
		public static int SCK_DOWN;
		[CCode (cname="SCK_UP")]
		public static int SCK_UP;
		[CCode (cname="SCK_LEFT")]
		public static int SCK_LEFT;
		[CCode (cname="SCK_RIGHT")]
		public static int SCK_RIGHT;
		[CCode (cname="SCK_HOME")]
		public static int SCK_HOME;
		[CCode (cname="SCK_END")]
		public static int SCK_END;
		[CCode (cname="SCK_PRIOR")]
		public static int SCK_PRIOR;
		[CCode (cname="SCK_NEXT")]
		public static int SCK_NEXT;
		[CCode (cname="SCK_DELETE")]
		public static int SCK_DELETE;
		[CCode (cname="SCK_INSERT")]
		public static int SCK_INSERT;
		[CCode (cname="SCK_ESCAPE")]
		public static int SCK_ESCAPE;
		[CCode (cname="SCK_BACK")]
		public static int SCK_BACK;
		[CCode (cname="SCK_TAB")]
		public static int SCK_TAB;
		[CCode (cname="SCK_RETURN")]
		public static int SCK_RETURN;
		[CCode (cname="SCK_ADD")]
		public static int SCK_ADD;
		[CCode (cname="SCK_SUBTRACT")]
		public static int SCK_SUBTRACT;
		[CCode (cname="SCK_DIVIDE")]
		public static int SCK_DIVIDE;
		[CCode (cname="SCK_WIN")]
		public static int SCK_WIN;
		[CCode (cname="SCK_RWIN")]
		public static int SCK_RWIN;
		[CCode (cname="SCK_MENU")]
		public static int SCK_MENU;
		[CCode (cname="SCMOD_NORM")]
		public static int SCMOD_NORM;
		[CCode (cname="SCMOD_SHIFT")]
		public static int SCMOD_SHIFT;
		[CCode (cname="SCMOD_CTRL")]
		public static int SCMOD_CTRL;
		[CCode (cname="SCMOD_ALT")]
		public static int SCMOD_ALT;
		[CCode (cname="SCMOD_SUPER")]
		public static int SCMOD_SUPER;
		[CCode (cname="SCMOD_META")]
		public static int SCMOD_META;
		[CCode (cname="SCN_STYLENEEDED")]
		public static int SCN_STYLENEEDED;
		[CCode (cname="SCN_CHARADDED")]
		public static int SCN_CHARADDED;
		[CCode (cname="SCN_SAVEPOINTREACHED")]
		public static int SCN_SAVEPOINTREACHED;
		[CCode (cname="SCN_SAVEPOINTLEFT")]
		public static int SCN_SAVEPOINTLEFT;
		[CCode (cname="SCN_MODIFYATTEMPTRO")]
		public static int SCN_MODIFYATTEMPTRO;
		[CCode (cname="SCN_KEY")]
		public static int SCN_KEY;
		[CCode (cname="SCN_DOUBLECLICK")]
		public static int SCN_DOUBLECLICK;
		[CCode (cname="SCN_UPDATEUI")]
		public static int SCN_UPDATEUI;
		[CCode (cname="SCN_MODIFIED")]
		public static int SCN_MODIFIED;
		[CCode (cname="SCN_MACRORECORD")]
		public static int SCN_MACRORECORD;
		[CCode (cname="SCN_MARGINCLICK")]
		public static int SCN_MARGINCLICK;
		[CCode (cname="SCN_NEEDSHOWN")]
		public static int SCN_NEEDSHOWN;
		[CCode (cname="SCN_PAINTED")]
		public static int SCN_PAINTED;
		[CCode (cname="SCN_USERLISTSELECTION")]
		public static int SCN_USERLISTSELECTION;
		[CCode (cname="SCN_URIDROPPED")]
		public static int SCN_URIDROPPED;
		[CCode (cname="SCN_DWELLSTART")]
		public static int SCN_DWELLSTART;
		[CCode (cname="SCN_DWELLEND")]
		public static int SCN_DWELLEND;
		[CCode (cname="SCN_ZOOM")]
		public static int SCN_ZOOM;
		[CCode (cname="SCN_HOTSPOTCLICK")]
		public static int SCN_HOTSPOTCLICK;
		[CCode (cname="SCN_HOTSPOTDOUBLECLICK")]
		public static int SCN_HOTSPOTDOUBLECLICK;
		[CCode (cname="SCN_CALLTIPCLICK")]
		public static int SCN_CALLTIPCLICK;
		[CCode (cname="SCN_AUTOCSELECTION")]
		public static int SCN_AUTOCSELECTION;
		[CCode (cname="SCN_INDICATORCLICK")]
		public static int SCN_INDICATORCLICK;
		[CCode (cname="SCN_INDICATORRELEASE")]
		public static int SCN_INDICATORRELEASE;
		[CCode (cname="SCN_AUTOCCANCELLED")]
		public static int SCN_AUTOCCANCELLED;
		[CCode (cname="SCN_AUTOCCHARDELETED")]
		public static int SCN_AUTOCCHARDELETED;
		[CCode (cname="SCN_HOTSPOTRELEASECLICK")]
		public static int SCN_HOTSPOTRELEASECLICK;
		[CCode (cname="SCN_FOCUSIN")]
		public static int SCN_FOCUSIN;
	}
