
[CCode(cheader_fileanme="geanygobject.h",
       cname="GeanyGObject",
       gir_namespace="GeanyGObject",
       gir_version="1.0")]
namespace GeanyGObject
{

	public class Document : GLib.Object
	{
		/* this is the actual GeanyDocument instance */
		private unowned GeanyRaw.Document _doc;
		private bool owns_doc;

		/* signals */
		public signal void closed();
		public signal void reloaded();
		public signal void before_save();
		public signal void saved();
		public signal void activate();
		public signal void filetype_set(GeanyRaw.Filetype old_ft);

		/* properties */
		public bool is_readonly {
			get { return _doc.readonly; }
			set
			{
				_doc.editor.sci.send_message(Scintilla.SCI_SETREADONLY, value?1:0, 0);
				_doc.readonly = value;
			}
		}

		public bool changed {
			get { return _doc.changed; }
			set { _doc.set_text_changed(value); }
		}

		public bool is_valid
		{
			get { return _doc.is_valid; }
		}

		public bool has_bom { get { return _doc.has_bom; } }
		public bool has_tags { get { return _doc.has_tags; } }
		public int  index { get { return _doc.index; } }

		public string display_name
		{
			owned get { return _doc.get_basename_for_display(); }
		}

		public string encoding
		{
			get { return _doc.encoding; }
			set { _doc.set_encoding(value); }
		}

		private Document()
		{
			GeanyRaw.geany_object.document_close.connect((doc)            => { if (doc == _doc) this.closed(); });
			GeanyRaw.geany_object.document_activate.connect((doc)         => { if (doc == _doc) this.activate(); });
			GeanyRaw.geany_object.document_filetype_set.connect((doc, ft) => { if (doc == _doc) this.filetype_set(ft); });
			GeanyRaw.geany_object.document_reload.connect((doc)           => { if (doc == _doc) this.reloaded(); });
			GeanyRaw.geany_object.document_before_save.connect((doc)      => { if (doc == _doc) this.before_save(); });
			GeanyRaw.geany_object.document_save.connect((doc)             => { if (doc == _doc) this.saved(); });
		}

		/* constructors */
		public Document.new_file(string? utf8_filename = null,
		                         GeanyRaw.Filetype? ft = null, string? text = null)
		{
			this();
			_doc = GeanyRaw.Document.new_file(utf8_filename, ft, text);
			owns_doc = true;
		}
		public Document.from_file(string locale_filename, bool readonly = false,
		                          GeanyRaw.Filetype? ft = null,  string? forced_enc = null)
		{
			this();
			_doc = GeanyRaw.Document.open_file(locale_filename, readonly, ft, forced_enc);
			owns_doc = true;
		}

		public static Document get_current()
		{
			var obj = new Document();
			obj._doc = GeanyRaw.Document.get_current();
			/* this only gets the reference to the current doc, must not close on
			 * destruction (note that GeanyDocument is not reference counted) */
			obj.owns_doc = false;
			return obj;
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

		/* instance methods */
		public bool close()
		{
			return _doc.close();
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
	}
}
