#include "ConsoleUtils.h"
#include "Logger.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace ftxui;

namespace {
    // All of our fields are single-line. ftxui's InputOption::multiline
    // defaults to true, and when it's true, pressing Enter inserts a
    // literal '\n' character into the bound string *in addition to* firing
    // on_enter - which is exactly what caused Enter to save a newline into
    // the database instead of just moving focus to the next field. Every
    // InputOption in this file must go through this helper.
    InputOption singleLineInput() {
        InputOption option;
        option.multiline = false;
        return option;
    }
}

#pragma region Constructor
ConsoleUtils::ConsoleUtils() {
    Logger::getInstance().log("ConsoleUtils constructor: building the UI.");
}
#pragma endregion

#pragma region Destructor
ConsoleUtils::~ConsoleUtils() {
    Logger::getInstance().log("ConsoleUtils destructor: tearing down the UI.");
}
#pragma endregion

#pragma region Authorization Ui
std::pair<std::string, std::string> ConsoleUtils::showLoginWindow(bool& exitRequested) {
    exitRequested = false;
    auto screen = ScreenInteractive::TerminalOutput();
    std::string login;
    std::string password;
    bool submitted = false;
    bool should_exit = false;

    auto login_button = Button("Start", [&] {
        submitted = true;
        screen.Exit();
        });

    auto exit_button = Button("Exit", [&] {
        should_exit = true;
        screen.Exit();
        });

    InputOption input_login_option = singleLineInput();
    InputOption input_password_option = singleLineInput();
    input_password_option.password = true;

    // Declared before input_login so its on_enter callback (set further
    // below, before input_login is constructed) can safely capture it by
    // reference: the callback only runs later, once the screen loop is
    // active, by which point input_password already holds a real component.
    Component input_password;

    // Pressing Enter in the login field moves focus to the password field,
    // and pressing Enter in the password field submits the form - matching
    // what users expect from a login form. NOTE: InputOption is copied into
    // the component at Input()-call time (InputBase stores it by value), so
    // on_enter (and multiline) must be assigned *before* calling Input(),
    // not after.
    input_login_option.on_enter = [&] {
        input_password->TakeFocus();
        };
    input_password_option.on_enter = [&] {
        submitted = true;
        screen.Exit();
        };

    auto input_login = Input(&login, "Login", input_login_option);
    input_password = Input(&password, "Password", input_password_option);

    auto button_renderer = Renderer(Container::Horizontal({ login_button, exit_button }), [&] {
        return hbox({
            login_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 30) | flex,
            text("  "),
            exit_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 30) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        input_login,
        input_password,
        button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Sign In") | bold | center,
            separator(),
            hbox(text("Login: "), input_login->Render()) | size(WIDTH, GREATER_THAN, 2),
            hbox(text("Password: "), input_password->Render()) | size(WIDTH, GREATER_THAN, 2),
            separator(),
            button_renderer->Render() | center,
            })
            // If the console window is smaller than this form, `frame`
            // turns it into a scrollable viewport instead of just
            // silently clipping the bottom of the content off-screen.
            | frame
            | vscroll_indicator;
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        exitRequested = true;
        return { "", "" };
    }

    if (should_exit) {
        exitRequested = true;
        return { "", "" };
    }

    return { login, password };
}
#pragma endregion

#pragma region Main UI
int ConsoleUtils::showMenuWindow(const std::vector<std::string>& entries) {
    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;

    // NOTE on why this isn't a plain ftxui Menu(&entries, &selected, ...)
    // anymore: MenuBase::OnMouseEvent (see ftxui/src/ftxui/component/menu.cpp)
    // only updates `selected`/`focused_entry` when you click an entry - it
    // never calls OnEnter(). Only pressing the Return key does. That's why
    // clicking used to highlight an item but do nothing until Enter was
    // pressed afterwards. Building the menu out of real Button() components
    // instead - the same component the login window's Start/Exit buttons
    // already use successfully with the mouse - makes a single click both
    // choose *and* confirm the entry, matching what a user expects and
    // matching what Enter already does for the keyboard.
    std::vector<Component> entryButtons;
    entryButtons.reserve(entries.size());
    for (int i = 0; i < static_cast<int>(entries.size()); ++i) {
        ButtonOption option;
        option.transform = [i, &selected](const EntryState& s) {
            std::string label = (selected == i ? "> " : "  ") + s.label;
            Element e = text(label);
            if (s.focused) {
                e = e | inverted;
            }
            return e;
            };
        auto button = Button(entries[i], [&screen, &selected, i] {
            selected = i;
            screen.Exit();
            }, option);
        entryButtons.push_back(button);
    }

    int max_length = 0;
    for (const auto& entry : entries) {
        max_length = std::max(max_length, static_cast<int>(entry.size()));
    }

    auto menu = Container::Vertical(entryButtons);

    auto renderer = Renderer(menu, [&] {
        Elements rows;
        for (auto& button : entryButtons) {
            rows.push_back(button->Render());
        }
        return vbox(std::move(rows))
            // `frame` turns this vbox into a scrollable viewport that
            // automatically scrolls to keep the focused entry visible.
            // Without it, a menu taller than the console window simply
            // got clipped at the bottom with no way to reach the rest.
            | frame
            | vscroll_indicator
            | border
            | size(WIDTH, GREATER_THAN, max_length + 2)
            | color(Color::Cyan);
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        return 1;
    }

    return selected;
}
#pragma endregion

#pragma region Add User Ui
NewUserInput ConsoleUtils::showAddUserWindow(const std::vector<std::string>& libraryNames) {
    auto screen = ScreenInteractive::TerminalOutput();

    NewUserInput result;
    bool submitted = false;
    bool cancelled = false;

    std::vector<std::string> roleOptions = { "admin", "librarian" };
    int selectedRole = 0;

    InputOption input_login_option = singleLineInput();
    InputOption input_password_option = singleLineInput();
    input_password_option.password = true;

    // See showLoginWindow() for why these are declared before the
    // corresponding on_enter callbacks are assigned.
    Component input_password;
    Component role_radiobox;
    Component input_fullName;

    input_login_option.on_enter = [&] {
        input_password->TakeFocus();
        };
    input_password_option.on_enter = [&] {
        role_radiobox->TakeFocus();
        };

    auto input_login = Input(&result.login, "Login", input_login_option);
    input_password = Input(&result.password, "Password", input_password_option);
    role_radiobox = Radiobox(&roleOptions, &selectedRole);

    // --- Librarian-only fields: which library the new librarian works at
    // and their work schedule. These are only shown (and only required)
    // when "librarian" is selected above; wrapped in Maybe() so they don't
    // clutter the form - or accept focus - when adding a plain admin.
    std::vector<std::string> dayOptions = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };
    int selectedStartDay = 0;  // Monday
    int selectedEndDay = 4;    // Friday
    int selectedLibrary = 0;

    InputOption fullName_option = singleLineInput();
    InputOption startTime_option = singleLineInput();
    InputOption endTime_option = singleLineInput();

    Component library_radiobox;
    Component startDay_radiobox;
    Component endDay_radiobox;
    Component input_startTime;
    Component input_endTime;

    fullName_option.on_enter = [&] { library_radiobox->TakeFocus(); };
    startTime_option.on_enter = [&] { input_endTime->TakeFocus(); };
    endTime_option.on_enter = [&] { submitted = true; screen.Exit(); };

    input_fullName = Input(&result.librarianFullName, "Librarian full name", fullName_option);
    library_radiobox = Radiobox(&libraryNames, &selectedLibrary);
    startDay_radiobox = Radiobox(&dayOptions, &selectedStartDay);
    endDay_radiobox = Radiobox(&dayOptions, &selectedEndDay);
    input_startTime = Input(&result.startTime, "Shift start, e.g. 09:00", startTime_option);
    input_endTime = Input(&result.endTime, "Shift end, e.g. 18:00", endTime_option);

    auto librarianFieldsContainer = Container::Vertical({
        input_fullName, library_radiobox, startDay_radiobox, endDay_radiobox, input_startTime, input_endTime
        });
    auto librarianFieldsIsVisible = [&] { return selectedRole == 1; };

    // Renderer(child, fn) forwards focus/keyboard events to `child` while
    // using `fn` for custom layout - wrapping librarianFieldsContainer here
    // (rather than adding it directly to another container too) keeps it
    // as a child of exactly one parent.
    auto librarianFieldsRenderer = Renderer(librarianFieldsContainer, [&] {
        return vbox({
            hbox(text("Full name: "), input_fullName->Render()),
            text("Library:"),
            libraryNames.empty()
                ? text("  (no libraries yet - add one first)") | color(Color::Red)
                : library_radiobox->Render(),
            text("Work week start day:"),
            startDay_radiobox->Render(),
            text("Work week end day:"),
            endDay_radiobox->Render(),
            hbox(text("Shift start: "), input_startTime->Render()),
            hbox(text("Shift end: "), input_endTime->Render()),
            });
        });
    auto librarianFields = Maybe(librarianFieldsRenderer, librarianFieldsIsVisible);

    auto submit_button = Button("Add", [&] {
        submitted = true;
        screen.Exit();
        });
    auto cancel_button = Button("Cancel", [&] {
        cancelled = true;
        screen.Exit();
        });

    auto button_renderer = Renderer(Container::Horizontal({ submit_button, cancel_button }), [&] {
        return hbox({
            submit_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex,
            text("  "),
            cancel_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        input_login,
        input_password,
        role_radiobox,
        librarianFields,
        button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Add New User") | bold | center,
            separator(),
            hbox(text("Login: "), input_login->Render()) | size(WIDTH, GREATER_THAN, 2),
            hbox(text("Password: "), input_password->Render()) | size(WIDTH, GREATER_THAN, 2),
            text("Role:"),
            role_radiobox->Render(),
            librarianFields->Render(),
            separator(),
            button_renderer->Render() | center,
            })
            | frame
            | vscroll_indicator;
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        result.cancelled = true;
        return result;
    }

    if (cancelled || !submitted) {
        result.cancelled = true;
        return result;
    }

    result.role = roleOptions[selectedRole];
    if (result.role == "librarian") {
        if (libraryNames.empty()) {
            // Nothing to pick from yet - report this clearly instead of
            // silently indexing into an empty list.
            std::cerr << "Cannot add a librarian: there must be at least one library first." << std::endl;
            result.cancelled = true;
            return result;
        }
        result.libraryName = libraryNames[selectedLibrary];
        result.startDay = std::to_string(selectedStartDay + 1);
        result.endDay = std::to_string(selectedEndDay + 1);
    }

    result.cancelled = false;
    return result;
}
#pragma endregion

#pragma region Add Book Ui
NewBookInput ConsoleUtils::showAddBookWindow() {
    auto screen = ScreenInteractive::TerminalOutput();

    NewBookInput result;
    bool submitted = false;
    bool cancelled = false;

    InputOption title_option = singleLineInput();
    InputOption author_option = singleLineInput();
    InputOption genre_option = singleLineInput();
    InputOption publisher_option = singleLineInput();
    InputOption year_option = singleLineInput();
    InputOption isbn_option = singleLineInput();

    // Declared up front (see showLoginWindow() for why): each on_enter
    // callback below captures the *next* field by reference, and that
    // field must exist as a variable before the on_enter option is copied
    // into its own Input() component.
    Component input_author, input_genre, input_publisher, input_year, input_isbn;
    Component submit_button;

    title_option.on_enter = [&] { input_author->TakeFocus(); };
    author_option.on_enter = [&] { input_genre->TakeFocus(); };
    genre_option.on_enter = [&] { input_publisher->TakeFocus(); };
    publisher_option.on_enter = [&] { input_year->TakeFocus(); };
    year_option.on_enter = [&] { input_isbn->TakeFocus(); };
    isbn_option.on_enter = [&] { submitted = true; screen.Exit(); };

    auto input_title = Input(&result.title, "Title", title_option);
    input_author = Input(&result.author, "Author", author_option);
    input_genre = Input(&result.genre, "Genre (optional)", genre_option);
    input_publisher = Input(&result.publisher, "Publisher (optional)", publisher_option);
    input_year = Input(&result.year, "Year (optional)", year_option);
    input_isbn = Input(&result.isbn, "ISBN (optional)", isbn_option);

    submit_button = Button("Add", [&] { submitted = true; screen.Exit(); });
    auto cancel_button = Button("Cancel", [&] { cancelled = true; screen.Exit(); });

    auto button_renderer = Renderer(Container::Horizontal({ submit_button, cancel_button }), [&] {
        return hbox({
            submit_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex,
            text("  "),
            cancel_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        input_title, input_author, input_genre, input_publisher, input_year, input_isbn, button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Add New Book") | bold | center,
            separator(),
            hbox(text("Title: "), input_title->Render()),
            hbox(text("Author: "), input_author->Render()),
            hbox(text("Genre: "), input_genre->Render()),
            hbox(text("Publisher: "), input_publisher->Render()),
            hbox(text("Year: "), input_year->Render()),
            hbox(text("ISBN: "), input_isbn->Render()),
            separator(),
            button_renderer->Render() | center,
            }) | size(WIDTH, GREATER_THAN, 2) | frame | vscroll_indicator;
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        result.cancelled = true;
        return result;
    }

    result.cancelled = cancelled || !submitted;
    return result;
}
#pragma endregion

#pragma region Add Reader Ui
NewReaderInput ConsoleUtils::showAddReaderWindow() {
    auto screen = ScreenInteractive::TerminalOutput();

    NewReaderInput result;
    bool submitted = false;
    bool cancelled = false;

    std::vector<std::string> categoryOptions = { "pupil", "student", "kindergartener", "employee", "researcher" };
    int selectedCategory = 0;

    InputOption name_option = singleLineInput();
    InputOption address_option = singleLineInput();
    InputOption phone_option = singleLineInput();
    InputOption institution_option = singleLineInput();
    InputOption major_option = singleLineInput();
    InputOption schoolNumber_option = singleLineInput();
    InputOption kindergarten_option = singleLineInput();
    InputOption research_option = singleLineInput();

    Component input_address, input_phone, category_radiobox, input_institution,
        input_major, input_schoolNumber, input_kindergarten, input_research;
    Component submit_button;

    name_option.on_enter = [&] { input_address->TakeFocus(); };
    address_option.on_enter = [&] { input_phone->TakeFocus(); };
    phone_option.on_enter = [&] { category_radiobox->TakeFocus(); };
    institution_option.on_enter = [&] { input_major->TakeFocus(); };
    major_option.on_enter = [&] { input_schoolNumber->TakeFocus(); };
    schoolNumber_option.on_enter = [&] { input_kindergarten->TakeFocus(); };
    kindergarten_option.on_enter = [&] { input_research->TakeFocus(); };
    research_option.on_enter = [&] { submitted = true; screen.Exit(); };

    auto input_name = Input(&result.name, "Name", name_option);
    input_address = Input(&result.address, "Address (optional)", address_option);
    input_phone = Input(&result.phone, "Phone (optional)", phone_option);
    category_radiobox = Radiobox(&categoryOptions, &selectedCategory);
    input_institution = Input(&result.institutionName, "Institution / employer (if relevant)", institution_option);
    input_major = Input(&result.major, "Major (students only)", major_option);
    input_schoolNumber = Input(&result.schoolNumber, "School number (pupils only)", schoolNumber_option);
    input_kindergarten = Input(&result.kindergartenName, "Kindergarten name (if relevant)", kindergarten_option);
    input_research = Input(&result.researchField, "Research field (researchers only)", research_option);

    submit_button = Button("Add", [&] { submitted = true; screen.Exit(); });
    auto cancel_button = Button("Cancel", [&] { cancelled = true; screen.Exit(); });

    auto button_renderer = Renderer(Container::Horizontal({ submit_button, cancel_button }), [&] {
        return hbox({
            submit_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex,
            text("  "),
            cancel_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        input_name, input_address, input_phone, category_radiobox,
        input_institution, input_major, input_schoolNumber, input_kindergarten, input_research,
        button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Add New Reader") | bold | center,
            separator(),
            hbox(text("Name: "), input_name->Render()),
            hbox(text("Address: "), input_address->Render()),
            hbox(text("Phone: "), input_phone->Render()),
            text("Category:"),
            category_radiobox->Render(),
            text("Only fill in the field(s) relevant to the chosen category:"),
            hbox(text("Institution/employer: "), input_institution->Render()),
            hbox(text("Major: "), input_major->Render()),
            hbox(text("School number: "), input_schoolNumber->Render()),
            hbox(text("Kindergarten: "), input_kindergarten->Render()),
            hbox(text("Research field: "), input_research->Render()),
            separator(),
            button_renderer->Render() | center,
            }) | size(WIDTH, GREATER_THAN, 2) | frame | vscroll_indicator;
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        result.cancelled = true;
        return result;
    }

    if (cancelled || !submitted) {
        result.cancelled = true;
        return result;
    }

    result.categoryType = categoryOptions[selectedCategory];
    result.cancelled = false;
    return result;
}
#pragma endregion

#pragma region Add Visit Ui
NewVisitInput ConsoleUtils::showAddVisitWindow(const std::vector<std::string>& readerNames,
    const std::vector<std::string>& libraryNames,
    const std::vector<std::string>& bookTitles) {
    auto screen = ScreenInteractive::TerminalOutput();

    NewVisitInput result;
    bool submitted = false;
    bool cancelled = false;

    // Reader and library must reference an existing row, and free-text
    // entry was error-prone (typos meant addVisit() would reject the
    // whole visit as "not found"). Both are now picked from a Radiobox
    // built from the actual database contents instead of typed by hand.
    if (readerNames.empty() || libraryNames.empty()) {
        // Nothing to pick from yet (e.g. no readers/libraries have been
        // added). Report this clearly instead of showing an empty,
        // unusable selection list.
        std::cerr << "Cannot add a visit: there must be at least one reader and one library first."
            << std::endl;
        result.cancelled = true;
        return result;
    }

    int selectedReader = 0;
    int selectedLibrary = 0;

    // "No book" is always the first option, since a visit doesn't have to
    // involve borrowing a book.
    std::vector<std::string> bookOptions = { "(no book)" };
    bookOptions.insert(bookOptions.end(), bookTitles.begin(), bookTitles.end());
    int selectedBook = 0;

    auto reader_radiobox = Radiobox(&readerNames, &selectedReader);
    auto library_radiobox = Radiobox(&libraryNames, &selectedLibrary);
    auto book_radiobox = Radiobox(&bookOptions, &selectedBook);

    auto submit_button = Button("Add", [&] { submitted = true; screen.Exit(); });
    auto cancel_button = Button("Cancel", [&] { cancelled = true; screen.Exit(); });

    auto button_renderer = Renderer(Container::Horizontal({ submit_button, cancel_button }), [&] {
        return hbox({
            submit_button->Render() | color(Color::Green) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex,
            text("  "),
            cancel_button->Render() | color(Color::Red) | borderRounded | size(WIDTH, GREATER_THAN, 20) | flex
            }) | center;
        });

    auto container = Container::Vertical({
        reader_radiobox, library_radiobox, book_radiobox, button_renderer
        });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Add New Visit") | bold | center,
            separator(),
            text("Reader:"),
            reader_radiobox->Render(),
            text("Library:"),
            library_radiobox->Render(),
            text("Book (optional):"),
            book_radiobox->Render(),
            separator(),
            button_renderer->Render() | center,
            }) | size(WIDTH, GREATER_THAN, 2) | frame | vscroll_indicator;
        });

    try {
        screen.Loop(renderer);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        result.cancelled = true;
        return result;
    }

    if (cancelled || !submitted) {
        result.cancelled = true;
        return result;
    }

    result.readerName = readerNames[selectedReader];
    result.libraryName = libraryNames[selectedLibrary];
    result.bookTitle = (selectedBook == 0) ? "" : bookOptions[selectedBook];
    result.cancelled = false;
    return result;
}
#pragma endregion
