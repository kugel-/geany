using GLib;
using Geany;

namespace GeanyGI
{
	public struct App
	{
		public bool         debug_mode;
		public weak string  configdir;
		public weak string  datadir;
		public weak string  docdir;

		internal App()
		{
			debug_mode = Geany.data.app.debug_mode;
			configdir  = Geany.data.app.configdir;
			datadir    = Geany.data.app.datadir;
			docdir     = Geany.data.app.docdir;
		}
	}

	public struct UiWidgets
	{
		public weak Gtk.Widget window;
		public weak Gtk.Widget toolbar;
		public weak Gtk.Widget sidebar_notebook;
		public weak Gtk.Widget notebook;
		public weak Gtk.Widget editor_menu;
		public weak Gtk.Widget tools_menu;
		public weak Gtk.Widget progressbar;
		public weak Gtk.Widget message_window_notebook;
		public weak Gtk.Widget project_menu;

		public UiWidgets()
		{
			window                  = Geany.data.main_widgets.window;
			toolbar                 = Geany.data.main_widgets.toolbar;
			sidebar_notebook        = Geany.data.main_widgets.sidebar_notebook;
			notebook                = Geany.data.main_widgets.notebook;
			editor_menu             = Geany.data.main_widgets.editor_menu;
			tools_menu              = Geany.data.main_widgets.tools_menu;
			progressbar             = Geany.data.main_widgets.progressbar;
			message_window_notebook = Geany.data.main_widgets.message_window_notebook;
			project_menu            = Geany.data.main_widgets.project_menu;
		}
	}

	public class Data : GLib.Object
	{
		public UiWidgets              widgets;
		public GenericArray<Document> all_documents;
		public GenericArray<Filetype> all_filetypes;
		/* shadow copoies because pygobject does not handle pointers-to-structs yet (unlike vala) */
		public App                    app;
		public Geany.Prefs            prefs;
		public Geany.InterfacePrefs   interface_prefs;
		public Geany.ToolPrefs        tool_prefs;
		public Geany.ToolbarPrefs     toolbar_prefs;
		public Geany.EditorPrefs      editor_prefs;
		public Geany.FilePrefs        file_prefs;
		public Geany.SearchPrefs      search_prefs;
		public Geany.TemplatePrefs    template_prefs;
		public Geany.BuildInfo        build_info;

		private void sync_prefs()
		{
			prefs = *Geany.data.prefs;
			interface_prefs = *Geany.data.interface_prefs;
			tool_prefs = *Geany.data.tool_prefs;
			toolbar_prefs = *Geany.data.toolbar_prefs;
			editor_prefs = *Geany.data.editor_prefs;
			file_prefs = *Geany.data.file_prefs;
			search_prefs = *Geany.data.search_prefs;
			template_prefs = *Geany.data.template_prefs;
			build_info = *Geany.data.build_info;
		}

		construct
		{
			app = App();
			widgets = UiWidgets();
			Geany.Object o = (Geany.Object) Geany.object;
			o.save_settings.connect(() => { sync_prefs(); });
			all_documents = new GenericArray<Document>();
			all_filetypes = new GenericArray<Filetype>();
		}

		private Data()
		{
			all_documents.length = (int) Geany.data.documents_array.length;
			Geany.data.documents_array.foreach((doc) => { if (doc.is_valid) all_documents.add(new Document(doc)); });
			all_filetypes.length = (int) Geany.data.filetypes_array.length;
			Geany.data.filetypes_array.foreach((ft) => { all_filetypes.add(new Filetype(ft)); });
			sync_prefs();
		}

		~Data()
		{
			_instance = null;
		}

		/* singleton impl */
		private static weak Data _instance;
		public  static Data instance()
		{
			Data _d;
			if (_instance == null)
			{
				/* vala doesn't let me do this is in a single expression */
				_d = new Data();
				_instance = _d;
			}
			return _instance;
		}
	}
}
