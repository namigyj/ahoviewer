#ifndef _BROWSER_H_
#define _BROWSER_H_

#include <gtkmm.h>

#include "imagefetcher.h"
#include "page.h"
#include "site.h"
#include "tagentry.h"
#include "tagview.h"

#include "settings.h"

namespace AhoViewer
{
    namespace Booru
    {
        class Browser : public Gtk::VPaned
        {
        public:
            typedef sigc::signal<void, Page*> SignalPageChangedType;

            Browser(BaseObjectType *cobj, const Glib::RefPtr<Gtk::Builder> &bldr);
            virtual ~Browser();

            Page* get_active_page() const
                { return static_cast<Page*>(m_Notebook->get_nth_page(m_Notebook->get_current_page())); }
            int get_selected_booru() const { return m_ComboBox->get_active_row_number(); }
            Gtk::Entry* get_tag_entry() const { return m_TagEntry; }
            ImageFetcher* get_image_fetcher() const { return m_ImageFetcher; }
            int get_min_width() const { return m_MinWidth; }

            void set_statusbar(StatusBar *sb) { m_StatusBar = sb; }

            SignalPageChangedType signal_page_changed() const { return m_SignalPageChanged; }

            // Action callbacks {{{
            void on_new_tab();
            void on_close_tab();
            void on_save_images();
            // }}}
        protected:
            virtual void on_realize();
        private:
            class ComboBoxModelColumns : public Gtk::TreeModel::ColumnRecord
            {
            public:
                ComboBoxModelColumns() { add(pixbuf_column); add(text_column); }
                Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> pixbuf_column;
                Gtk::TreeModelColumn<std::string> text_column;
            };

            void on_entry_activate();
            void on_page_closed(Page *page);
            void on_page_removed(Gtk::Widget*, guint);
            void on_switch_page(void*, guint);
            void on_imagelist_changed(const std::shared_ptr<AhoViewer::Image> &image);

            std::string readable_file_size(double s);

            std::shared_ptr<Site> get_active_site() const { return Settings.get_sites()[m_ComboBox->get_active_row_number()]; }

            StatusBar *m_StatusBar;

            Gtk::ToolButton *m_NewTabButton,
                            *m_SaveImagesButton;
            Gtk::ComboBox *m_ComboBox;
            Gtk::Notebook *m_Notebook;

            ComboBoxModelColumns m_ComboColumns;
            Glib::RefPtr<Gtk::ListStore> m_ComboModel;

            TagEntry *m_TagEntry;
            TagView *m_TagView;

            bool m_IgnorePageSwitch;
            int m_MinWidth;

            ImageFetcher *m_ImageFetcher;
            Glib::RefPtr<Gtk::UIManager> m_UIManager;
            Glib::RefPtr<Gtk::Action> m_SaveImagesAction;

            sigc::connection m_NoResultsConn,
                             m_ImageListConn,
                             m_ImageProgConn;

            SignalPageChangedType m_SignalPageChanged;
        };
    }
}

#endif /* _BROWSER_H_ */