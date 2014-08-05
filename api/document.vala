using Geany;

namespace GeanyGI
{
	public int loadhelper()
	{
		return 42;
	}

	public class Document : GLib.Object
	{
		/* fields */
		private        bool    owns_doc;
		public         Editor  editor;

		/* properties */
		public bool is_readonly
		{
			get
			{
				return _doc.readonly;
			}
			set
			{
				_doc.editor.sci.send_message(Scintilla.SCI_SETREADONLY, value?1:0, 0);
				_doc.readonly = value;
			}
		}

		public bool changed
		{
			get { return _doc.changed; }
			set { _doc.set_text_changed(value); }
		}

		public string encoding
		{
			get { return _doc.encoding; }
			set { _doc.set_encoding(value); }
		}

		public bool   is_valid      { get { return _doc.is_valid; } }
		public bool   has_bom       { get { return _doc.has_bom; } }
		public bool   has_tags      { get { return _doc.has_tags; } }
		public int    index         { get { return _doc.index; } }
		public string display_name  { owned get { return _doc.get_basename_for_display(); } }
		public Scintilla.Object sci { get { return _doc.editor.sci; }}

		/* signals */
		public signal void closed();
		public signal void reloaded();
		public signal void before_save();
		public signal void saved();
		public signal void activate();
		public signal void filetype_set(Filetype? old_ft);

		/* constructors */
		construct
		{
			/* cast necessary to access signals */
			Geany.Object o = (Geany.Object) Geany.object;
			o.document_close.connect((doc)            => { if (doc == _doc) this.closed(); });
			o.document_activate.connect((doc)         => { if (doc == _doc) this.activate(); });
			o.document_filetype_set.connect((doc, ft) =>
			{
				if (doc == _doc)
				{
					/* can signal with ft == null, e.g. during Geany.Document.new_file() */
					GeanyGI.Filetype gi_ft = ft != null ? Filetype.get_by_id(ft.id) : null;
					this.filetype_set(gi_ft);
				}
			});
			o.document_reload.connect((doc)           => { if (doc == _doc) this.reloaded(); });
			o.document_before_save.connect((doc)      => { if (doc == _doc) this.before_save(); });
			o.document_save.connect((doc)             => { if (doc == _doc) this.saved(); });
		}

		internal Document(Geany.Document doc)
		{
			_doc = doc;
			editor = new GeanyGI.Editor.create(this);
			/* this only gets the reference to the current doc, must not close on
			 * destruction (note that GeanyDocument is not reference counted) */
			owns_doc = false;
		}

		public Document.new_file(string? utf8_filename = null,
								 Filetype? ft = null,
								 string? text = null)
		{
			unowned Geany.Filetype gft = ft != null ? ft._ft : null;
			_doc = Geany.Document.new_file(utf8_filename, gft, text);
			editor = new GeanyGI.Editor.create(this);
			owns_doc = true;
		}
		public Document.from_file(string locale_filename, bool readonly = false,
								  Filetype? ft = null, string? forced_enc = null)
		{
			unowned Geany.Filetype gft = ft != null ? ft._ft : null;
			_doc = Geany.Document.open_file(locale_filename, readonly, gft, forced_enc);
			editor = new GeanyGI.Editor.create(this);
			owns_doc = true;
		}

		public static Document? get_current()
		{
			return new Document(Geany.Document.get_current());
		}

		/* destructor */
		~Document()
		{
			if (owns_doc)
			{
				if (!close())
				{
					/* force close if user didnt save */
					changed = false;
					close();
				}
			}
		}

		public bool close()
		{
			/* on shutdown it might be closed before the plugin can close it in the cleanup method */
			if (! is_valid)
				return true;
			owns_doc = !_doc.close();
			return !owns_doc;
		}

		public bool save(bool force = false)
		{
			return _doc.save_file(force);
		}

		public bool save_as(string? file_name = null)
		{
			return _doc.save_file_as(file_name);
		}

		public bool reload(string? forced_enc = null)
		{
			return _doc.reload_file(forced_enc);
		}

		/* there is no proper introsepection for static fields, workaround with accessor */
		private static Signals _signals;
		public static Signals signals()
		{
			if (_signals == null)
				_signals = new Signals();
			return _signals;
		}

		/* just wraps Geany.object signals */
		public class Signals : GLib.Object
		{
			public signal void document_new(Document doc);
			public signal void document_open(Document doc);
			public signal void document_reload(Document doc);
			public signal void document_before_save(Document doc);
			public signal void document_save(Document doc);
			public signal void document_filetype_set(Document doc, Filetype? old_ft);
			public signal void document_activate(Document doc);
			public signal void document_close(Document doc);

			construct
			{
				Geany.Object o = (Geany.Object) Geany.object;
				o.document_new.connect((doc)              => { this.document_new(new Document(doc)); });
				o.document_open.connect((doc)             => { this.document_open(new Document(doc)); });
				o.document_close.connect((doc)            => { this.document_close(new Document(doc)); });
				o.document_activate.connect((doc)         => { this.document_activate(new Document(doc)); });
				o.document_filetype_set.connect((doc, ft) =>
				{
					/* can signal with ft == null, e.g. during Geany.Document.new_file() */
					Filetype gi_ft = ft != null ? Filetype.get_by_id(ft.id) : null;
					this.document_filetype_set(new Document(doc), gi_ft);
				});
				o.document_reload.connect((doc)           => { this.document_reload(new Document(doc)); });
				o.document_before_save.connect((doc)      => { this.document_before_save(new Document(doc)); });
				o.document_save.connect((doc)             => { this.document_save(new Document(doc)); });
			}
		}

		/* internal fields
		 * must be last due to bug in vala's gir generation */

		/* this is the actual GeanyDocument instance */
		internal unowned Geany.Document _doc;
	}
}
