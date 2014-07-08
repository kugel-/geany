using GeanyRaw;

namespace Geany
{
	public class Window : GLib.Object
	{
		HashTable<unowned Document?, Doc> docs;

		construct
		{
			docs = new HashTable<unowned Document?, Doc>(
				direct_hash, direct_equal);
		}

		unowned Doc add_or_lookup(Document gdoc)
		{
			unowned Doc? doc = docs.lookup(gdoc);
			if (doc != null)
				return doc;
			Doc new_doc = new Doc(gdoc);
			docs.insert(gdoc, new_doc);
			GeanyRaw.geany_object.document_close.connect(on_document_close);
			GeanyRaw.geany_object.document_new.connect(on_document_new);
			GeanyRaw.geany_object.document_open.connect(on_document_opened);
			GeanyRaw.geany_object.document_activate.connect(on_document_activated);
			GeanyRaw.geany_object.document_filetype_set.connect(on_document_filetype_set);
			GeanyRaw.geany_object.document_reload.connect(on_document_reloaded);
			GeanyRaw.geany_object.document_save.connect(on_document_saved);
			return docs.lookup(gdoc);
		}

		// Properties
		public Doc active_document
		{
			get { return add_or_lookup(Document.get_current()); }
		}

		public List<unowned Doc> documents
		{
			owned get { return docs.get_values(); }
		}

		public List<unowned Doc> unsaved_documents
		{
			owned get
			{
				var doc_list = new List<unowned Doc>();
				foreach (unowned Doc doc in docs.get_values())
				{
					if (doc.is_changed)
						doc_list.append(doc);
				}
				return doc_list;
			}
		}

		internal bool is_doc_valid(Doc doc)
		{
			return (docs.lookup(doc.doc_ptr) != null);
		}

		// Signals
		public signal void document_closed(Doc doc);
		public signal void document_added(Doc doc);
		public signal void document_opened(Doc doc);
		public signal void document_new(Doc doc);
		public signal void document_activated(Doc doc);
		public signal void document_reloaded(Doc doc);
		public signal void document_saved(Doc doc);
		public signal void document_filetype_changed(Doc doc, FileType old_ft);

		// Signal handlers
		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_close(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			doc.closed();
			document_closed(doc);
			docs.remove(gdoc);
		}

		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_new(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			document_new(doc);
			document_added(doc);
		}

		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_opened(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			document_opened(doc);
			document_added(doc);
		}

		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_activated(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			doc.activated();
			document_activated(doc);
		}

		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_reloaded(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			doc.reloaded();
			document_reloaded(doc);
		}

		[CCode(target_pos=0.9, instance_pos=2.1)]
		void on_document_saved(GeanyRaw.Object obj, Document gdoc)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			doc.saved();
			document_saved(doc);
		}

		[CCode(target_pos=0.9, instance_pos=3.1)]
		void on_document_filetype_set(GeanyRaw.Object obj, Document gdoc, Filetype old_ft)
		{
			unowned Doc doc = add_or_lookup(gdoc);
			unowned FileType ft = FileTypeManager.get_default().lookup_ft(old_ft);
			doc.filetype_changed(ft);
			document_filetype_changed(doc, ft);
		}

		// Singleton stuff
		Window() {}
		static Window inst = new Window();
		public static Window get_default() { return inst; }
	}

}
