#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <algorithm>

// ── Static members ────────────────────────────────────────
int User::nextId           = 1001;
int Booking::nextBookingId = 5001;

// ── Style ─────────────────────────────────────────────────
const QString MainWindow::QSS = R"(
QWidget          { background:#C9D9F0;
                   font-family:"Segoe UI",Arial,sans-serif; }
QScrollArea      { border:none; background:#C9D9F0; }
QScrollBar:vertical         { background:#C9D9F0; width:6px;
                              border-radius:3px; }
QScrollBar::handle:vertical { background:#0D2149; border-radius:3px;
                              min-height:30px; }
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical { height:0px; }
QFrame#Card     { background:#0D2149; border-radius:18px; }
QFrame#Header   { background:#0D2149; }
QLabel#Title    { color:#0D2149; font-size:28px; font-weight:900;
                  background:transparent; }
QLabel#Sub      { color:#0D2149; font-size:14px;
                  background:transparent; }
QLabel#FieldLbl { color:#0D2149; font-size:13px; font-weight:800;
                  background:transparent; }
QLabel#White    { color:white; font-size:14px;
                  background:transparent; }
QLabel#WhiteBold{ color:white; font-size:15px; font-weight:800;
                  background:transparent; }
QLabel#Logo     { color:white; font-size:15px; font-weight:900;
                  background:transparent; }
QLabel#Error    { color:#C0281C; font-size:11px;
                  background:transparent; }
QLabel#Success  { color:#0D6E0A; font-size:12px; font-weight:700;
                  background:transparent; }
QLineEdit       { background:white; border:none; border-radius:12px;
                  color:#0D2149; font-size:13px; padding:10px 14px; }
QComboBox       { background:white; border:none; border-radius:12px;
                  color:#0D2149; font-size:13px; padding:10px 14px; }
QListWidget     { background:white; border-radius:12px;
                  color:#0D2149; font-size:13px; padding:6px; }
QListWidget::item:selected { background:#0D2149; color:white;
                             border-radius:8px; }
QTextEdit       { background:white; border-radius:12px;
                  color:#0D2149; font-size:13px; padding:8px; }
QPushButton#Main { background:#0D2149; color:white; border:none;
                   border-radius:14px; font-size:13px;
                   font-weight:800; padding:12px 0; }
QPushButton#Main:hover   { background:#1A3A6B; }
QPushButton#Main:pressed { background:#091830; }
QPushButton#Link { background:transparent; border:none;
                   color:#4A6FA5; font-size:13px; font-weight:600; }
QPushButton#Link:hover { color:#0D2149; }
QPushButton#Back { background:transparent; border:none;
                   color:#4A6FA5; font-size:14px; font-weight:700; }
QPushButton#Send { background:#0D2149; color:white; border:none;
                   border-radius:14px; font-size:13px;
                   font-weight:700; padding:10px 18px; }
QPushButton#Small{ background:#0D2149; color:white; border:none;
                   border-radius:12px; font-size:12px;
                   font-weight:800; padding:8px 0; }
)";

// ── Constructor ───────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Mofix");
    setFixedWidth(420);
    setMinimumHeight(750);
    setStyleSheet(QSS);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    splashPage       = buildSplashPage();
    loginPage        = buildLoginPage();
    registerPage     = buildRegisterPage();
    customerHomePage = buildCustomerHomePage();
    workerHomePage   = buildWorkerHomePage();
    workerListPage   = buildWorkerListPage();
    bookingPage      = buildBookingPage();
    myBookingsPage   = buildMyBookingsPage();
    trackPage        = buildTrackPage();
    reviewPage       = buildReviewPage();
    statusPage       = buildStatusPage();
    profilePage      = buildProfilePage();
    chatPage         = buildChatPage();
    recommendPage    = buildRecommendPage();

    for (QWidget* p : {splashPage, loginPage, registerPage,
                       customerHomePage, workerHomePage,
                       workerListPage,   bookingPage,
                       myBookingsPage,   trackPage,
                       reviewPage,       statusPage,
                       profilePage,      chatPage,
                       recommendPage})
        stack->addWidget(p);

    // Stay on splash until user clicks "Fix it now"
    stack->setCurrentWidget(splashPage);
}

MainWindow::~MainWindow() { delete ui; }

// ── Helpers ───────────────────────────────────────────────
void MainWindow::goTo(QWidget* page)
{ stack->setCurrentWidget(page); }

QPushButton* MainWindow::makeBtn(const QString& txt,
                                 const QString& obj,
                                 QWidget* parent)
{
    auto* b = new QPushButton(txt, parent);
    b->setObjectName(obj);
    b->setCursor(Qt::PointingHandCursor);
    return b;
}

QScrollArea* MainWindow::makeScroll(QWidget*& inner)
{
    auto* s = new QScrollArea();
    s->setWidgetResizable(true);
    s->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    inner = new QWidget();
    s->setWidget(inner);
    return s;
}

// ── SPLASH ────────────────────────────────────────────────
QWidget* MainWindow::buildSplashPage()
{
    auto* page = new QWidget();
    page->setStyleSheet("background:#0D2149;");
    auto* v = new QVBoxLayout(page);
    v->setContentsMargins(30,40,30,40);

    // University name – top left
    auto* uniRow  = new QHBoxLayout();
    auto* uniIcon = new QLabel("◈");
    uniIcon->setStyleSheet(
        "color:#A8C0E8; font-size:20px; background:transparent;");
    auto* uniName = new QLabel("Innovation\nUniversity");
    uniName->setStyleSheet(
        "color:#A8C0E8; font-size:13px; font-weight:700;"
        "background:transparent;");
    uniRow->addWidget(uniIcon);
    uniRow->addWidget(uniName);
    uniRow->addStretch();
    v->addLayout(uniRow);

    v->addStretch();

    auto* title = new QLabel("Mofix");
    title->setStyleSheet(
        "color:white; font-size:80px; font-weight:900;");
    title->setAlignment(Qt::AlignCenter);

    auto* sub = new QLabel("Your Home, Fixed in a Click");
    sub->setStyleSheet(
        "color:#A8C0E8; font-size:17px; font-style:italic;");
    sub->setAlignment(Qt::AlignCenter);

    v->addWidget(title);
    v->addWidget(sub);
    v->addSpacing(80);

    // Only navigate when user clicks the button
    auto* btn = makeBtn("Fix it now","Main");
    btn->setFixedSize(200,50);
    connect(btn, &QPushButton::clicked, [=](){ goTo(loginPage); });
    v->addWidget(btn, 0, Qt::AlignCenter);

    v->addStretch();
    return page;
}

// ── LOGIN ─────────────────────────────────────────────────
QWidget* MainWindow::buildLoginPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(32,60,32,40); v->setSpacing(10);

    auto* title = new QLabel("Welcome Back");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);
    auto* sub = new QLabel("Log in to Mofix");
    sub->setObjectName("Sub");
    sub->setAlignment(Qt::AlignCenter);
    v->addWidget(title); v->addWidget(sub); v->addSpacing(24);

    auto* eLbl = new QLabel("Email"); eLbl->setObjectName("FieldLbl");
    v->addWidget(eLbl);
    loginEmail = new QLineEdit();
    loginEmail->setPlaceholderText("you@email.com");
    v->addWidget(loginEmail); v->addSpacing(6);

    auto* pLbl = new QLabel("Password"); pLbl->setObjectName("FieldLbl");
    v->addWidget(pLbl);
    loginPass = new QLineEdit();
    loginPass->setEchoMode(QLineEdit::Password);
    loginPass->setPlaceholderText("••••••••");
    v->addWidget(loginPass); v->addSpacing(4);

    loginError = new QLabel("");
    loginError->setObjectName("Error");
    loginError->hide();
    v->addWidget(loginError);

    v->addSpacing(16);
    auto* loginBtn = makeBtn("Log In","Main");
    loginBtn->setFixedHeight(50);
    connect(loginBtn, &QPushButton::clicked,
            this, &MainWindow::onLoginClicked);
    v->addWidget(loginBtn);

    auto* regLink = makeBtn(
        "Don't have an account? Create one","Link");
    connect(regLink, &QPushButton::clicked,
            [=](){ goTo(registerPage); });
    v->addWidget(regLink, 0, Qt::AlignCenter);
    v->addStretch();
    return scroll;
}

// ── REGISTER ──────────────────────────────────────────────
QWidget* MainWindow::buildRegisterPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(32,30,32,40); v->setSpacing(8);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(loginPage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("Create Account");
    title->setObjectName("Title");
    v->addWidget(title); v->addSpacing(10);

    auto addF = [&](const QString& lbl, QLineEdit*& ref,
                    const QString& ph, bool pw = false)
    {
        auto* l = new QLabel(lbl);
        l->setObjectName("FieldLbl");
        v->addWidget(l);
        ref = new QLineEdit();
        ref->setPlaceholderText(ph);
        if (pw) ref->setEchoMode(QLineEdit::Password);
        v->addWidget(ref); v->addSpacing(4);
    };

    addF("Full Name",    regName,  "Enter your name");
    addF("Email",        regEmail, "you@email.com");
    addF("Password",     regPass,  "••••••••", true);
    addF("Phone Number", regPhone, "01XXXXXXXXX");
    addF("Address/City", regAddr,  "Your city");

    auto* tLbl = new QLabel("Register As");
    tLbl->setObjectName("FieldLbl");
    v->addWidget(tLbl);
    regType = new QComboBox();
    regType->addItems({"Customer","Service Worker"});
    v->addWidget(regType); v->addSpacing(4);

    regCategoryLbl = new QLabel("Service Category");
    regCategoryLbl->setObjectName("FieldLbl");
    v->addWidget(regCategoryLbl);
    regCategory = new QComboBox();
    regCategory->addItems({"Plumbing","Electrician","Carpenter",
                           "Deep Cleaning","AC Repair","Appliances"});
    v->addWidget(regCategory);
    regCategoryLbl->hide(); regCategory->hide();

    connect(regType,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int i){
                regCategoryLbl->setVisible(i == 1);
                regCategory->setVisible(i == 1);
            });

    v->addSpacing(14);
    auto* createBtn = makeBtn("CREATE ACCOUNT","Main");
    createBtn->setFixedHeight(50);
    connect(createBtn, &QPushButton::clicked,
            this, &MainWindow::onRegisterClicked);
    v->addWidget(createBtn);
    v->addStretch();
    return scroll;
}

// ── CUSTOMER HOME ─────────────────────────────────────────
QWidget* MainWindow::buildCustomerHomePage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(0,0,0,20); v->setSpacing(0);

    // Header
    auto* hdr = new QFrame(); hdr->setObjectName("Header");
    hdr->setFixedHeight(68);
    auto* hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(20,0,20,0);
    custWelcome = new QLabel("Hello!");
    custWelcome->setObjectName("WhiteBold");
    auto* logo = new QLabel("Mofix");
    logo->setObjectName("Logo");
    hl->addWidget(custWelcome);
    hl->addStretch();
    hl->addWidget(logo);
    v->addWidget(hdr);

    auto* body  = new QWidget();
    auto* bv    = new QVBoxLayout(body);
    bv->setContentsMargins(16,14,16,14); bv->setSpacing(10);

    // ── Search bar ────────────────────────────────────────
    auto* searchRow = new QHBoxLayout(); searchRow->setSpacing(8);
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("🔍  What do you need help with?");
    searchInput->setFixedHeight(44);
    searchInput->setStyleSheet(
        "background:white; border:none; border-radius:14px;"
        "color:#0D2149; font-size:13px; padding:0 14px;");
    auto* searchBtn = makeBtn("Search","Main");
    searchBtn->setFixedSize(80,44);
    connect(searchBtn,  &QPushButton::clicked,
            this, &MainWindow::onSearch);
    connect(searchInput,&QLineEdit::returnPressed,
            this, &MainWindow::onSearch);
    searchRow->addWidget(searchInput);
    searchRow->addWidget(searchBtn);
    bv->addLayout(searchRow);

    searchMsg = new QLabel("");
    searchMsg->setObjectName("Error");
    searchMsg->setWordWrap(true);
    searchMsg->hide();
    bv->addWidget(searchMsg);

    // ── Services title ────────────────────────────────────
    auto* sLbl = new QLabel("Our Services");
    sLbl->setObjectName("Title");
    bv->addWidget(sLbl);

    // ── 6 cards – 3 per row ───────────────────────────────
    QStringList cats  = {"Plumbing","Electrician","Carpenter",
                        "Deep Cleaning","AC Repair","Appliances"};
    QStringList icons = {"🔧","⚡","🪚","🧹","❄️","🔩"};

    for (int i = 0; i < cats.size(); i += 3) {
        auto* row = new QHBoxLayout(); row->setSpacing(8);
        for (int j = i; j < i+3 && j < cats.size(); j++) {
            auto* card = new QFrame();
            card->setObjectName("Card");
            card->setFixedHeight(90);
            auto* cl = new QVBoxLayout(card);
            cl->setContentsMargins(4,8,4,8);
            cl->setSpacing(4);

            auto* ic = new QLabel(icons[j]);
            ic->setStyleSheet(
                "color:white; font-size:20px; background:transparent;");
            ic->setAlignment(Qt::AlignCenter);

            QString cat = cats[j];
            auto* cb = makeBtn(cat,"Link",card);
            cb->setStyleSheet(
                "background:transparent; border:none; color:white;"
                "font-size:11px; font-weight:800;");
            connect(cb, &QPushButton::clicked,
                    [=](){ showWorkerList(cat); });

            cl->addWidget(ic);
            cl->addWidget(cb);
            row->addWidget(card);
        }
        bv->addLayout(row);
    }

    // ── Discount banner – below services ─────────────────
    bv->addSpacing(4);
    auto* banner = new QFrame(); banner->setObjectName("Card");
    banner->setFixedHeight(56);
    auto* bl = new QHBoxLayout(banner);
    bl->setContentsMargins(16,0,16,0);
    auto* bTxt = new QLabel(
        "🛒  Weekend offer: 20% OFF on all services!");
    bTxt->setObjectName("WhiteBold");
    bTxt->setAlignment(Qt::AlignCenter);
    bl->addWidget(bTxt);
    bv->addWidget(banner);

    // ── Bottom buttons – smaller ──────────────────────────
    bv->addSpacing(6);

    auto* r1 = new QHBoxLayout(); r1->setSpacing(8);
    auto* bBk = makeBtn("My Bookings","Small");
    auto* bCh = makeBtn("AI Chat","Small");
    bBk->setFixedHeight(40); bCh->setFixedHeight(40);
    connect(bBk, &QPushButton::clicked,
            [=](){ refreshMyBookings(); goTo(myBookingsPage); });
    connect(bCh, &QPushButton::clicked,
            [=](){ goTo(chatPage); });
    r1->addWidget(bBk); r1->addWidget(bCh);
    bv->addLayout(r1);

    auto* r2 = new QHBoxLayout(); r2->setSpacing(8);
    auto* bTp = makeBtn("AI Tips","Small");
    auto* bPr = makeBtn("My Profile","Small");
    bTp->setFixedHeight(40); bPr->setFixedHeight(40);
    connect(bTp, &QPushButton::clicked,
            this, &MainWindow::onAIRecommendation);
    connect(bPr, &QPushButton::clicked,
            [=](){ updateProfilePage(); goTo(profilePage); });
    r2->addWidget(bTp); r2->addWidget(bPr);
    bv->addLayout(r2);

    auto* out = makeBtn("Logout","Link");
    out->setFixedHeight(32);
    connect(out, &QPushButton::clicked, this, &MainWindow::onLogout);
    bv->addWidget(out, 0, Qt::AlignCenter);

    v->addWidget(body);
    return scroll;
}

// ── WORKER HOME ───────────────────────────────────────────
QWidget* MainWindow::buildWorkerHomePage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(0,0,0,20); v->setSpacing(0);

    auto* hdr = new QFrame(); hdr->setObjectName("Header");
    hdr->setFixedHeight(68);
    auto* hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(20,0,20,0);
    workerWelcome = new QLabel("Hello!");
    workerWelcome->setObjectName("WhiteBold");
    auto* logo = new QLabel("Mofix"); logo->setObjectName("Logo");
    hl->addWidget(workerWelcome); hl->addStretch(); hl->addWidget(logo);
    v->addWidget(hdr);

    auto* body = new QWidget(); auto* bv = new QVBoxLayout(body);
    bv->setContentsMargins(22,20,22,20); bv->setSpacing(12);

    auto* title = new QLabel("Worker Dashboard");
    title->setObjectName("Title"); bv->addWidget(title);

    auto* bBk  = makeBtn("My Bookings","Main");
    auto* bSt  = makeBtn("Update Booking Status","Main");
    auto* bAv  = makeBtn("Toggle My Availability","Main");
    auto* bCh  = makeBtn("AI Chatbot","Main");
    auto* bPr  = makeBtn("My Profile","Main");
    auto* bOut = makeBtn("Logout","Link");

    connect(bBk, &QPushButton::clicked,
            [=](){ refreshWorkerBookings(); goTo(statusPage); });
    connect(bSt, &QPushButton::clicked,
            [=](){ refreshWorkerBookings(); goTo(statusPage); });
    connect(bAv, &QPushButton::clicked,
            this, &MainWindow::onToggleAvailability);
    connect(bCh, &QPushButton::clicked,
            [=](){ goTo(chatPage); });
    connect(bPr, &QPushButton::clicked,
            [=](){ updateProfilePage(); goTo(profilePage); });
    connect(bOut,&QPushButton::clicked,
            this, &MainWindow::onLogout);

    for (auto* b : {bBk,bSt,bAv,bCh,bPr}) {
        b->setFixedHeight(50); bv->addWidget(b);
    }
    bv->addWidget(bOut, 0, Qt::AlignCenter);
    bv->addStretch();
    v->addWidget(body);
    return scroll;
}

// ── WORKER LIST ───────────────────────────────────────────
QWidget* MainWindow::buildWorkerListPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(16,16,16,16); v->setSpacing(12);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(customerHomePage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("Available Professionals");
    title->setObjectName("Title"); v->addWidget(title);

    workerListWidget = new QListWidget();
    workerListWidget->setMinimumHeight(300);
    // Card-style items
    workerListWidget->setStyleSheet(
        "QListWidget { background:white; border-radius:14px; padding:8px; }"
        "QListWidget::item { background:#0D2149; color:white;"
        "  border-radius:12px; padding:14px 12px;"
        "  margin:5px 2px; font-size:13px; font-weight:600; }"
        "QListWidget::item:selected { background:#1A3A6B; }");
    v->addWidget(workerListWidget);

    auto* bookBtn = makeBtn("Book Now","Main");
    bookBtn->setFixedHeight(50);
    connect(bookBtn, &QPushButton::clicked, [=](){
        int row = workerListWidget->currentRow();
        if (row >= 0) selectWorker(row);
        else QMessageBox::warning(this,"",
                                 "Please select a worker first.");
    });
    v->addWidget(bookBtn);
    v->addStretch();
    return scroll;
}

// ── BOOKING ───────────────────────────────────────────────
QWidget* MainWindow::buildBookingPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(10);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(workerListPage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("Book Now");
    title->setObjectName("Title"); v->addWidget(title);

    bookWorkerInfo = new QLabel("");
    bookWorkerInfo->setObjectName("FieldLbl");
    bookWorkerInfo->setWordWrap(true);
    v->addWidget(bookWorkerInfo);

    bookDiscountLabel = new QLabel(
        "** Weekend Offer: 20% OFF applied! **");
    bookDiscountLabel->setObjectName("Success");
    bookDiscountLabel->hide();
    v->addWidget(bookDiscountLabel);

    bookPriceLabel = new QLabel("");
    bookPriceLabel->setObjectName("FieldLbl");
    v->addWidget(bookPriceLabel);

    auto* slotLbl = new QLabel("Select Time Slot");
    slotLbl->setObjectName("FieldLbl"); v->addWidget(slotLbl);
    bookTimeSlot = new QComboBox();
    bookTimeSlot->addItems({"9 AM - 11 AM","11 AM - 1 PM",
                            "2 PM - 4 PM","5 PM - 7 PM"});
    v->addWidget(bookTimeSlot);

    auto* descLbl = new QLabel("Describe Your Problem");
    descLbl->setObjectName("FieldLbl"); v->addWidget(descLbl);
    bookDescription = new QLineEdit();
    bookDescription->setPlaceholderText("Optional description...");
    v->addWidget(bookDescription);

    v->addSpacing(14);
    auto* confirmBtn = makeBtn("Confirm Booking","Main");
    confirmBtn->setFixedHeight(50);
    connect(confirmBtn, &QPushButton::clicked,
            this, &MainWindow::onBookNow);
    v->addWidget(confirmBtn);
    v->addStretch();
    return scroll;
}

// ── MY BOOKINGS ───────────────────────────────────────────
QWidget* MainWindow::buildMyBookingsPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(12);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(customerHomePage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("My Bookings");
    title->setObjectName("Title"); v->addWidget(title);

    myBookingsList = new QListWidget();
    myBookingsList->setMinimumHeight(300);
    v->addWidget(myBookingsList);

    auto* row = new QHBoxLayout();
    auto* trackBtn  = makeBtn("Track Status","Main");
    auto* reviewBtn = makeBtn("Leave Review","Main");
    connect(trackBtn,  &QPushButton::clicked,
            this, &MainWindow::onTrackBooking);
    connect(reviewBtn, &QPushButton::clicked,
            this, &MainWindow::onLeaveReview);
    row->addWidget(trackBtn); row->addWidget(reviewBtn);
    v->addLayout(row);
    v->addStretch();
    return scroll;
}

// ── TRACK ─────────────────────────────────────────────────
QWidget* MainWindow::buildTrackPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(12);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(myBookingsPage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("Booking Tracker");
    title->setObjectName("Title"); v->addWidget(title);

    trackStatusLabel = new QLabel("");
    trackStatusLabel->setObjectName("FieldLbl");
    trackStatusLabel->setWordWrap(true);
    trackStatusLabel->setMinimumHeight(200);
    v->addWidget(trackStatusLabel);
    v->addStretch();
    return scroll;
}

// ── REVIEW ────────────────────────────────────────────────
QWidget* MainWindow::buildReviewPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(10);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(myBookingsPage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("Leave a Review");
    title->setObjectName("Title"); v->addWidget(title);

    auto* rLbl = new QLabel("Rating");
    rLbl->setObjectName("FieldLbl"); v->addWidget(rLbl);
    reviewRating = new QComboBox();
    reviewRating->addItems({"5 ⭐⭐⭐⭐⭐","4 ⭐⭐⭐⭐",
                            "3 ⭐⭐⭐","2 ⭐⭐","1 ⭐"});
    v->addWidget(reviewRating);

    auto* cLbl = new QLabel("Comment");
    cLbl->setObjectName("FieldLbl"); v->addWidget(cLbl);
    reviewComment = new QLineEdit();
    reviewComment->setPlaceholderText("Share your experience...");
    v->addWidget(reviewComment);

    reviewMsg = new QLabel("");
    reviewMsg->setObjectName("Success"); reviewMsg->hide();
    v->addWidget(reviewMsg);

    auto* submitBtn = makeBtn("Submit Review","Main");
    submitBtn->setFixedHeight(48);
    connect(submitBtn, &QPushButton::clicked,
            this, &MainWindow::onSubmitReview);
    v->addWidget(submitBtn);
    v->addStretch();
    return scroll;
}

// ── STATUS PAGE (Worker) ──────────────────────────────────
QWidget* MainWindow::buildStatusPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(12);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(workerHomePage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("My Bookings");
    title->setObjectName("Title"); v->addWidget(title);

    statusBookingList = new QListWidget();
    statusBookingList->setMinimumHeight(260);
    v->addWidget(statusBookingList);

    auto* sLbl = new QLabel("Update Status To:");
    sLbl->setObjectName("FieldLbl"); v->addWidget(sLbl);
    statusCombo = new QComboBox();
    statusCombo->addItems({"Pending","In Progress",
                           "Service Completed"});
    v->addWidget(statusCombo);

    statusMsg = new QLabel("");
    statusMsg->setObjectName("Success"); statusMsg->hide();
    v->addWidget(statusMsg);

    auto* upBtn = makeBtn("Update Status","Main");
    upBtn->setFixedHeight(48);
    connect(upBtn, &QPushButton::clicked,
            this, &MainWindow::onUpdateBookingStatus);
    v->addWidget(upBtn);
    v->addStretch();
    return scroll;
}

// ── PROFILE ───────────────────────────────────────────────
QWidget* MainWindow::buildProfilePage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(10);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked, [=](){
        if (currentUser && currentUser->getUserType()=="Customer")
            goTo(customerHomePage);
        else goTo(workerHomePage);
    });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("My Profile");
    title->setObjectName("Title"); v->addWidget(title);

    profName = new QLabel(""); profName->setObjectName("FieldLbl");
    profType = new QLabel(""); profType->setObjectName("FieldLbl");
    v->addWidget(profName); v->addWidget(profType);
    v->addSpacing(8);

    auto* pLbl = new QLabel("Phone");
    pLbl->setObjectName("FieldLbl"); v->addWidget(pLbl);
    profPhone = new QLineEdit();
    profPhone->setPlaceholderText("Phone"); v->addWidget(profPhone);

    auto* aLbl = new QLabel("Address");
    aLbl->setObjectName("FieldLbl"); v->addWidget(aLbl);
    profAddr = new QLineEdit();
    profAddr->setPlaceholderText("Address"); v->addWidget(profAddr);

    auto* pwLbl = new QLabel("Change Password (optional)");
    pwLbl->setObjectName("FieldLbl"); v->addWidget(pwLbl);
    profPass = new QLineEdit();
    profPass->setPlaceholderText("New password");
    profPass->setEchoMode(QLineEdit::Password);
    v->addWidget(profPass);

    profSaveMsg = new QLabel("");
    profSaveMsg->setObjectName("Success"); profSaveMsg->hide();
    v->addWidget(profSaveMsg);

    auto* saveBtn = makeBtn("Save Changes","Main");
    saveBtn->setFixedHeight(48);
    connect(saveBtn, &QPushButton::clicked,
            this, &MainWindow::onSaveProfile);
    v->addWidget(saveBtn);
    v->addStretch();
    return scroll;
}

// ── CHAT ──────────────────────────────────────────────────
QWidget* MainWindow::buildChatPage()
{
    auto* page = new QWidget();
    auto* v = new QVBoxLayout(page);
    v->setContentsMargins(0,0,0,0); v->setSpacing(0);

    auto* hdr = new QFrame(); hdr->setObjectName("Header");
    hdr->setFixedHeight(60);
    auto* hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(12,0,12,0);
    auto* backBtn = makeBtn("←","Back");
    backBtn->setStyleSheet(
        "color:white; font-size:18px;"
        "background:transparent; border:none;");
    connect(backBtn, &QPushButton::clicked, [=](){
        if (currentUser && currentUser->getUserType()=="Customer")
            goTo(customerHomePage);
        else goTo(workerHomePage);
    });
    auto* cTitle = new QLabel("MofBot — AI Assistant");
    cTitle->setObjectName("WhiteBold");
    cTitle->setAlignment(Qt::AlignCenter);
    hl->addWidget(backBtn);
    hl->addWidget(cTitle,1);
    hl->addSpacing(30);
    v->addWidget(hdr);

    chatDisplay = new QTextEdit();
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet(
        "background:#EFF3FA; border:none; padding:10px;");
    v->addWidget(chatDisplay,1);

    auto* inputArea = new QWidget();
    inputArea->setStyleSheet("background:#D0DCF0;");
    inputArea->setFixedHeight(58);
    auto* il = new QHBoxLayout(inputArea);
    il->setContentsMargins(10,8,10,8);
    chatInput = new QLineEdit();
    chatInput->setPlaceholderText("Ask me anything...");
    auto* sendBtn = makeBtn("Send","Send");
    connect(sendBtn,    &QPushButton::clicked,
            this, &MainWindow::onSendChat);
    connect(chatInput,  &QLineEdit::returnPressed,
            this, &MainWindow::onSendChat);
    il->addWidget(chatInput); il->addWidget(sendBtn);
    v->addWidget(inputArea);

    chatDisplay->append(
        "<b style='color:#0D2149'>MofBot:</b> "
        "Hello! I'm your Mofix AI assistant. "
        "Ask me about services, pricing, booking, or anything else!");
    return page;
}

// ── RECOMMEND ─────────────────────────────────────────────
QWidget* MainWindow::buildRecommendPage()
{
    QWidget* inner;
    auto* scroll = makeScroll(inner);
    auto* v = new QVBoxLayout(inner);
    v->setContentsMargins(24,20,24,20); v->setSpacing(12);

    auto* back = makeBtn("← Back","Back");
    connect(back, &QPushButton::clicked,
            [=](){ goTo(customerHomePage); });
    v->addWidget(back, 0, Qt::AlignLeft);

    auto* title = new QLabel("AI Recommendations");
    title->setObjectName("Title"); v->addWidget(title);

    recommendLabel = new QLabel("");
    recommendLabel->setObjectName("FieldLbl");
    recommendLabel->setWordWrap(true);
    v->addWidget(recommendLabel);

    recommendDisplay = new QTextEdit();
    recommendDisplay->setReadOnly(true);
    recommendDisplay->setMinimumHeight(220);
    v->addWidget(recommendDisplay);
    v->addStretch();
    return scroll;
}

// ── SLOTS ─────────────────────────────────────────────────
void MainWindow::onLoginClicked()
{
    loginError->hide();
    if (loginEmail->text().isEmpty() || loginPass->text().isEmpty()) {
        loginError->setText("Please fill all fields.");
        loginError->show(); return;
    }
    User* u = platform.login(
        loginEmail->text().trimmed(), loginPass->text());
    if (!u) {
        loginError->setText("Invalid email or password.");
        loginError->show(); return;
    }
    currentUser = u;
    if (u->getUserType() == "Customer") {
        custWelcome->setText("Hello, " + u->getName() + " !");
        goTo(customerHomePage);
    } else {
        workerWelcome->setText("Hello, " + u->getName() + " !");
        goTo(workerHomePage);
    }
}

void MainWindow::onRegisterClicked()
{
    if (regName->text().isEmpty()  || regEmail->text().isEmpty() ||
        regPass->text().isEmpty()  || regPhone->text().isEmpty() ||
        regAddr->text().isEmpty()) {
        QMessageBox::warning(this,"Error",
                             "Please fill all fields."); return;
    }
    if (regType->currentIndex() == 0) {
        auto* c = platform.registerCustomer(
            regName->text(), regEmail->text(),
            regPass->text(), regPhone->text(), regAddr->text());
        currentUser = c;
        custWelcome->setText("Hello, " + c->getName() + " !");
        goTo(customerHomePage);
    } else {
        double prices[] = {100,120,150,200,180,130};
        int idx = regCategory->currentIndex();
        auto* w = platform.registerWorker(
            regName->text(), regEmail->text(),
            regPass->text(), regPhone->text(), regAddr->text(),
            regCategory->currentText(), prices[idx]);
        currentUser = w;
        workerWelcome->setText("Hello, " + w->getName() + " !");
        goTo(workerHomePage);
    }
}

void MainWindow::onLogout()
{
    currentUser = nullptr; selectedWorker = nullptr;
    loginEmail->clear(); loginPass->clear();
    goTo(loginPage);
}

void MainWindow::onSearch()
{
    searchMsg->hide();
    QString query = searchInput->text().trimmed().toLower();
    if (query.isEmpty()) {
        searchMsg->setText("Please enter a service name to search.");
        searchMsg->show(); return;
    }
    QStringList cats = {"Plumbing","Electrician","Carpenter",
                        "Deep Cleaning","AC Repair","Appliances"};
    QString matched;
    for (const auto& c : cats) {
        if (c.toLower().contains(query) ||
            query.contains(c.toLower().split(" ")[0])) {
            matched = c; break;
        }
    }
    if (matched.isEmpty()) {
        searchMsg->setText(
            "No service found for \"" + searchInput->text() +
            "\". Try: Plumbing, Electrician, Carpenter, "
            "Cleaning, AC, Appliances.");
        searchMsg->show(); return;
    }
    showWorkerList(matched);
}

void MainWindow::showWorkerList(const QString& category)
{
    currentCategory = category;
    workerListWidget->clear();
    auto workers = platform.getWorkersByCategory(category);
    if (workers.isEmpty()) {
        workerListWidget->addItem(
            "No available workers in this category right now.");
    } else {
        for (auto* w : workers) {
            QString stars;
            int fs = (int)w->getRating();
            for (int i = 0; i < fs; i++) stars += "⭐";
            workerListWidget->addItem(
                QString("👤  %1\n"
                        "    Rating: %2  %3  "
                        "|  Price: %4 EGP  "
                        "|  Jobs: %5")
                    .arg(w->getName())
                    .arg(w->getRating(), 0,'f',1)
                    .arg(stars)
                    .arg(w->getPrice())
                    .arg(w->getCompletedJobs()));
        }
    }
    goTo(workerListPage);
}

void MainWindow::selectWorker(int row)
{
    auto workers = platform.getWorkersByCategory(currentCategory);
    if (row >= workers.size()) return;
    selectedWorker    = workers[row];
    bool   weekend    = platform.isWeekend();
    double price      = selectedWorker->getPrice()
                   * (weekend ? 0.8 : 1.0);

    bookWorkerInfo->setText(
        "Worker   : " + selectedWorker->getName() + "\n"
                                                    "Category : " + selectedWorker->getCategory() + "\n"
                                          "Rating   : " + QString::number(
            selectedWorker->getRating(),'f',1) + " / 5.0\n"
          "Jobs Done: " + QString::number(
            selectedWorker->getCompletedJobs()));

    bookDiscountLabel->setVisible(weekend);
    bookPriceLabel->setText(
        "Price: " + QString::number(price,'f',0) + " EGP"
        + (weekend ? "  (20% OFF applied!)" : ""));
    goTo(bookingPage);
}

void MainWindow::onBookNow()
{
    if (!selectedWorker || !currentUser) return;
    Customer* c = static_cast<Customer*>(currentUser);
    QString desc = bookDescription->text().trimmed();
    if (desc.isEmpty())
        desc = selectedWorker->getCategory() + " service request";

    Booking* b = platform.createBooking(
        c, selectedWorker, desc,
        bookTimeSlot->currentText(), platform.isWeekend());

    QMessageBox::information(this,"Request Sent Successfully!",
                             "✓  Request Sent Successfully!\n\n" +
                                 selectedWorker->getName() +
                                 " will contact you soon.\n"
                                 "Booking ID: " +
                                 QString::number(b->getBookingId()));

    selectedWorker = nullptr;
    bookDescription->clear();
    goTo(customerHomePage);
}

void MainWindow::refreshMyBookings()
{
    myBookingsList->clear();
    if (!currentUser || currentUser->getUserType()!="Customer") return;
    for (auto* b :
         static_cast<Customer*>(currentUser)->getBookings())
        myBookingsList->addItem(
            QString("[%1]  %2  |  %3  |  %4 EGP  |  %5")
                .arg(b->getBookingId())
                .arg(b->getService().getCategory())
                .arg(b->getWorkerName())
                .arg(b->getFinalPrice(), 0,'f',0)
                .arg(b->getStatus()));
}

void MainWindow::onTrackBooking()
{
    if (!currentUser) return;
    auto& books =
        static_cast<Customer*>(currentUser)->getBookings();
    int row = myBookingsList->currentRow();
    if (row < 0 || row >= books.size()) {
        QMessageBox::warning(this,"",
                             "Please select a booking first."); return;
    }
    Booking* b     = books[row];
    QString status = b->getStatus();
    trackStatusLabel->setText(
        "Booking ID : " +
        QString::number(b->getBookingId()) + "\n"
                                             "Worker     : " + b->getWorkerName()   + "\n"
                               "Service    : " +
        b->getService().getCategory()       + "\n"
                                        "Date       : " + b->getDate()          + "\n"
                         "Time Slot  : " + b->getTimeSlot()      + "\n\n"
                             "══════════════════════\n"
                             "[" + (status=="Pending"           ?"●":" ") +
        "]  Pending\n"
        "[" + (status=="In Progress"       ?"●":" ") +
        "]  In Progress\n"
        "[" + (status=="Service Completed" ?"●":" ") +
        "]  Service Completed\n\n"
        "Current Status:  " + status);
    goTo(trackPage);
}

void MainWindow::onLeaveReview()
{
    if (!currentUser) return;
    auto& books =
        static_cast<Customer*>(currentUser)->getBookings();
    int row = myBookingsList->currentRow();
    if (row < 0 || row >= books.size()) {
        QMessageBox::warning(this,"",
                             "Please select a booking first."); return;
    }
    if (books[row]->getStatus() != "Service Completed") {
        QMessageBox::warning(this,"",
                             "Can only review completed services."); return;
    }
    reviewMsg->hide(); reviewComment->clear();
    goTo(reviewPage);
}

void MainWindow::onSubmitReview()
{
    if (!currentUser) return;
    auto& books =
        static_cast<Customer*>(currentUser)->getBookings();
    int row = myBookingsList->currentRow();
    if (row < 0 || row >= books.size()) return;
    Booking* b      = books[row];
    double   rating = 5.0 - reviewRating->currentIndex();
    QString  comment= reviewComment->text().trimmed();
    if (comment.isEmpty()) comment = "Good service.";

    for (auto* w : platform.getWorkers())
        if (w->getId() == b->getWorkerId()) {
            w->addReview(Review(w->getId(),
                                currentUser->getId(),
                                currentUser->getName(),
                                rating, comment));
            break;
        }
    reviewMsg->setText("✓ Review submitted! Thank you.");
    reviewMsg->show();
}

void MainWindow::refreshWorkerBookings()
{
    statusBookingList->clear();
    if (!currentUser || currentUser->getUserType()!="Worker") return;
    for (auto* b :
         static_cast<Worker*>(currentUser)->getBookings())
        statusBookingList->addItem(
            QString("[%1]  %2  |  %3  |  %4")
                .arg(b->getBookingId())
                .arg(b->getService().getCategory())
                .arg(b->getTimeSlot())
                .arg(b->getStatus()));
}

void MainWindow::onUpdateBookingStatus()
{
    if (!currentUser || currentUser->getUserType()!="Worker") return;
    Worker* w  = static_cast<Worker*>(currentUser);
    int row    = statusBookingList->currentRow();
    if (row < 0 || row >= w->getBookings().size()) {
        statusMsg->setText("Please select a booking first.");
        statusMsg->show(); return;
    }
    Booking* b      = w->getBookings()[row];
    QString newStat = statusCombo->currentText();
    b->setStatus(newStat);
    if (newStat == "Service Completed")
        w->completeJob(b->getFinalPrice());
    statusMsg->setText("✓ Status updated to: " + newStat);
    statusMsg->show();
    refreshWorkerBookings();
}

void MainWindow::onToggleAvailability()
{
    if (!currentUser || currentUser->getUserType()!="Worker") return;
    Worker* w = static_cast<Worker*>(currentUser);
    w->setAvailability(!w->getAvailable());
    QMessageBox::information(this,"Availability Updated",
                             QString("You are now: ") +
                                 (w->getAvailable() ? "✅ Available" : "🔴 Busy"));
}

void MainWindow::onSendChat()
{
    QString text = chatInput->text().trimmed();
    if (text.isEmpty()) return;
    chatDisplay->append(
        "<b style='color:#333'>You:</b>  " + text);
    chatInput->clear();
    QString reply = platform.getChatBot().getReply(text);
    QTimer::singleShot(350, [=](){
        chatDisplay->append(
            "<b style='color:#0D2149'>MofBot:</b>  " +
            reply + "<br>");
    });
}

void MainWindow::onAIRecommendation()
{
    if (!currentUser || currentUser->getUserType()!="Customer") return;
    Customer* c      = static_cast<Customer*>(currentUser);
    QString   suggest= platform.getRecommender()
                          .suggestService(
                              c->getServiceHistory());
    if (suggest.isEmpty()) suggest = "Plumbing";
    recommendLabel->setText(
        "Based on your history, we recommend:  " + suggest);
    recommendDisplay->clear();
    recommendDisplay->append(
        "=== AI Tips for " + suggest + " ===\n");
    for (const auto& tip :
         platform.getRecommender().getTips(suggest))
        recommendDisplay->append("•  " + tip);
    goTo(recommendPage);
}

void MainWindow::onSaveProfile()
{
    if (!currentUser) return;
    if (!profPhone->text().isEmpty())
        currentUser->updatePhone(profPhone->text());
    if (!profAddr->text().isEmpty())
        currentUser->updateAddress(profAddr->text());
    if (!profPass->text().isEmpty())
        currentUser->updatePassword(profPass->text());
    profSaveMsg->setText("✓ Changes saved successfully!");
    profSaveMsg->show();
    profPass->clear();
}

void MainWindow::updateProfilePage()
{
    if (!currentUser) return;
    profName->setText("Name  :  " + currentUser->getName());
    profType->setText("Type   :  " + currentUser->getUserType());
    profPhone->setText(currentUser->getPhone());
    profAddr->setText(currentUser->getAddress());
    profSaveMsg->hide();
}