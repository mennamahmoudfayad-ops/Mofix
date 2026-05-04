#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDate>
#include <QTimer>

    QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ============================================================
// USER
// ============================================================
class User {
protected:
    QString name, email, password, phone, address;
    int id;
    static int nextId;
public:
    User(QString n, QString e, QString p, QString ph, QString addr)
        : name(n), email(e), password(p), phone(ph), address(addr)
    { id = nextId++; }
    virtual ~User() {}
    virtual QString getUserType() const = 0;
    QString getName()    const { return name; }
    QString getEmail()   const { return email; }
    QString getPhone()   const { return phone; }
    QString getAddress() const { return address; }
    int     getId()      const { return id; }
    bool checkPassword(const QString& p) const { return password == p; }
    void updatePhone(const QString& ph)        { phone    = ph; }
    void updateAddress(const QString& a)       { address  = a; }
    void updatePassword(const QString& p)      { password = p; }
};

// ============================================================
// SERVICE
// ============================================================
class Service {
private:
    QString category, description, status;
    double  basePrice;
public:
    Service() : basePrice(0), status("Pending") {}
    Service(QString cat, QString desc, double price)
        : category(cat), description(desc),
        basePrice(price), status("Pending") {}
    QString getCategory()    const { return category; }
    QString getDescription() const { return description; }
    double  getBasePrice()   const { return basePrice; }
    QString getStatus()      const { return status; }
    void    setStatus(const QString& s) { status = s; }
};

// ============================================================
// BOOKING
// ============================================================
class Booking {
private:
    static int nextBookingId;
    int     bookingId, customerId, workerId;
    QString workerName, timeSlot, bookingStatus, date;
    Service service;
    double  finalPrice;
public:
    Booking(int cId, int wId, QString wName,
            Service svc, QString slot, double price)
        : customerId(cId), workerId(wId), workerName(wName),
        service(svc), timeSlot(slot), finalPrice(price),
        bookingStatus("Pending")
    {
        bookingId = nextBookingId++;
        date = QDate::currentDate().toString("yyyy-MM-dd");
    }
    int     getBookingId()  const { return bookingId; }
    int     getCustomerId() const { return customerId; }
    int     getWorkerId()   const { return workerId; }
    QString getWorkerName() const { return workerName; }
    QString getStatus()     const { return bookingStatus; }
    QString getTimeSlot()   const { return timeSlot; }
    QString getDate()       const { return date; }
    double  getFinalPrice() const { return finalPrice; }
    Service getService()    const { return service; }
    void    setStatus(const QString& s) { bookingStatus = s; }
};

// ============================================================
// REVIEW
// ============================================================
class Review {
private:
    int     workerId, customerId;
    QString customerName, comment;
    double  rating;
public:
    Review(int wId, int cId, QString cName, double r, QString c)
        : workerId(wId), customerId(cId), customerName(cName),
        rating(r), comment(c) {}
    int     getWorkerId()     const { return workerId; }
    double  getRating()       const { return rating; }
    QString getCustomerName() const { return customerName; }
    QString getComment()      const { return comment; }
};

// ============================================================
// WORKER
// ============================================================
class Worker : public User {
private:
    QString  category;
    double   pricePerJob, rating, platformFeePercent;
    int      totalRatings, totalEarnings, completedJobs;
    bool     isAvailable;
    QVector<Review>   reviews;
    QVector<Booking*> myBookings;
public:
    Worker(QString n, QString e, QString p, QString ph,
           QString addr, QString cat, double price)
        : User(n,e,p,ph,addr), category(cat), pricePerJob(price),
        rating(0.0), totalRatings(0), isAvailable(true),
        totalEarnings(0), completedJobs(0), platformFeePercent(15.0) {}
    QString getUserType()      const override { return "Worker"; }
    QString getCategory()      const { return category; }
    double  getPrice()         const { return pricePerJob; }
    double  getRating()        const { return rating; }
    bool    getAvailable()     const { return isAvailable; }
    int     getCompletedJobs() const { return completedJobs; }
    int     getTotalEarnings() const { return totalEarnings; }
    QVector<Booking*>& getBookings() { return myBookings; }
    QVector<Review>&   getReviews()  { return reviews; }
    void setAvailability(bool a) { isAvailable = a; }
    void addBooking(Booking* b)  { myBookings.push_back(b); }
    void addReview(const Review& r) {
        reviews.push_back(r);
        totalRatings++;
        double sum = 0;
        for (const auto& rv : reviews) sum += rv.getRating();
        rating = sum / totalRatings;
    }
    void completeJob(double earned) {
        completedJobs++;
        totalEarnings += static_cast<int>(
            earned * (1.0 - platformFeePercent/100.0));
    }
    Booking* findBooking(int bid) {
        for (auto* b : myBookings)
            if (b->getBookingId() == bid) return b;
        return nullptr;
    }
};

// ============================================================
// CUSTOMER
// ============================================================
class Customer : public User {
private:
    QVector<Booking*> myBookings;
    QVector<QString>  serviceHistory;
public:
    Customer(QString n, QString e, QString p, QString ph, QString addr)
        : User(n,e,p,ph,addr) {}
    QString getUserType() const override { return "Customer"; }
    void addBooking(Booking* b) {
        myBookings.push_back(b);
        serviceHistory.push_back(b->getService().getCategory());
    }
    QVector<Booking*>&      getBookings()       { return myBookings; }
    const QVector<QString>& getServiceHistory() const { return serviceHistory; }
    Booking* findBooking(int bid) {
        for (auto* b : myBookings)
            if (b->getBookingId() == bid) return b;
        return nullptr;
    }
};

// ============================================================
// CHATBOT  (AI Feature #1)
// ============================================================
class ChatBot {
private:
    QString botName;
    QVector<QPair<QString,QString>> kb;
    void build() {
        kb = {
              {"hello",     "Hello! Welcome to MofIx. How can I help you?"},
              {"hi",        "Hi! I'm MofBot, your MofIx assistant!"},
              {"plumbing",  "Plumbing starts at 90 EGP — leaks, pipes, toilets!"},
              {"electric",  "Electrical services start at 110 EGP. Certified engineers!"},
              {"carpenter", "Carpentry starts at 140 EGP — furniture, doors, woodwork!"},
              {"cleaning",  "Deep cleaning starts at 180 EGP with pro equipment."},
              {"ac",        "AC repair starts at 160 EGP — cleaning, gas refill, full repair."},
              {"appliance", "Appliance repair starts at 120 EGP — washers, fridges, ovens."},
              {"price",     "Plumbing:90+, Electrical:110+, Carpentry:140+, "
                        "Cleaning:180+, AC:160+, Appliances:120+ EGP."},
              {"book",      "Browse Services → choose category → select worker → Book Now!"},
              {"cancel",    "Go to My Bookings and select the booking to cancel."},
              {"discount",  "Weekend special: 20% OFF on all services!"},
              {"weekend",   "Yes! 20% discount every Friday & Saturday. Book now!"},
              {"safe",      "All MofIx workers are background-checked and verified."},
              {"time",      "Services available from 8 AM to 9 PM daily."},
              {"area",      "We serve 10th of Ramadan City and nearby areas."},
              {"status",    "Track bookings: Pending → In Progress → Completed."},
              {"rating",    "After service is completed, rate your worker 1-5 stars."},
              {"payment",   "We accept cash on delivery. Online payment coming soon!"},
              {"thanks",    "You're welcome! Anything else I can help with?"},
              {"bye",       "Goodbye! Thank you for using MofIx!"},
              {"help",      "I can help with: pricing, booking, workers, discounts. Just ask!"},
              {"search",    "Use the search bar on the home page to find any service quickly!"},
              {"university","MofIx is a project by Innovation University students."},
              };
    }
public:
    ChatBot(QString name = "MofBot") : botName(name) { build(); }
    QString getReply(const QString& input) const {
        QString lower = input.toLower();
        for (const auto& qa : kb)
            if (lower.contains(qa.first)) return qa.second;
        return "Try asking about: pricing, booking, workers, or discounts. Type 'help'!";
    }
    QString getBotName() const { return botName; }
};

// ============================================================
// RECOMMENDATION ENGINE  (AI Feature #2)
// ============================================================
class RecommendationEngine {
private:
    QMap<QString, QVector<QString>> tips;
    void build() {
        tips["Plumbing"]      = {"Check for leaks monthly to save on bills.",
                            "Avoid pouring grease down drains.",
                            "Install a water filter for better quality."};
        tips["Electrician"]   = {"Never overload power sockets.",
                               "Switch to LED bulbs — save 75% energy.",
                               "Schedule electrical inspection every 2 years."};
        tips["Carpenter"]     = {"Treat wooden furniture annually.",
                             "Use silica gel to prevent moisture damage.",
                             "Tighten loose hinges before they cause damage."};
        tips["Deep Cleaning"] = {"Deep clean every 3 months for best hygiene.",
                                 "Clean AC filters monthly.",
                                 "Use microfiber cloths — trap 99% bacteria."};
        tips["AC Repair"]     = {"Clean filters monthly for efficiency.",
                             "Set AC to 24-26C to save electricity.",
                             "Service your AC before summer."};
        tips["Appliances"]    = {"Clean washing machine drum monthly.",
                              "Defrost freezer when ice exceeds 1cm.",
                              "Clean fridge coils annually."};
    }
public:
    RecommendationEngine() { build(); }
    QVector<QString> getTips(const QString& cat) const {
        return tips.contains(cat) ? tips[cat] : QVector<QString>();
    }
    QString suggestService(const QVector<QString>& history) const {
        if (history.isEmpty()) return "Plumbing";
        QMap<QString,int> freq;
        for (const auto& s : history) freq[s]++;
        QString best; int mx = 0;
        for (auto it = freq.begin(); it != freq.end(); ++it)
            if (it.value() > mx) { mx = it.value(); best = it.key(); }
        return best;
    }
};

// ============================================================
// PLATFORM
// ============================================================
class Platform {
private:
    QVector<Customer*> customers;
    QVector<Worker*>   workers;
    QVector<Booking*>  allBookings;
    double             totalRevenue;
    double             platformFee;
    ChatBot            chatbot;
    RecommendationEngine recommender;

    void seedWorkers() {
        // ── Plumbing ──────────────────────────────────────
        auto* w1 = new Worker("Eng. Ahmed Mohamed","ahmed@mofix.com",
                              "Pass@123","01008273716",
                              "10th of Ramadan","Plumbing",100.0);
        w1->addReview(Review(w1->getId(),9999,"Previous Client",4.9,"Excellent work!"));

        auto* w7 = new Worker("Eng. Karim Saad","karim@mofix.com",
                              "Pass@111","01123456789",
                              "10th of Ramadan","Plumbing",90.0);
        w7->addReview(Review(w7->getId(),9999,"Sara K.",4.5,"Good and fast!"));

        // ── Electrician ───────────────────────────────────
        auto* w2 = new Worker("Eng. Mohamed Ali","mali@mofix.com",
                              "Pass@456","01112345678",
                              "10th of Ramadan","Electrician",120.0);
        w2->addReview(Review(w2->getId(),9999,"Previous Client",4.7,"Very professional."));

        auto* w8 = new Worker("Eng. Youssef Nour","youssef@mofix.com",
                              "Pass@222","01234561111",
                              "10th of Ramadan","Electrician",110.0);
        w8->addReview(Review(w8->getId(),9999,"Ahmed R.",4.3,"Did a great job."));

        // ── Carpenter ─────────────────────────────────────
        auto* w3 = new Worker("Khaled Hassan","khaled@mofix.com",
                              "Pass@789","01234567890",
                              "10th of Ramadan","Carpenter",150.0);
        w3->addReview(Review(w3->getId(),9999,"Mariam",4.6,"Very skilled!"));

        auto* w9 = new Worker("Tarek Fahmy","tarek@mofix.com",
                              "Pass@333","01098761234",
                              "10th of Ramadan","Carpenter",140.0);

        // ── Deep Cleaning ─────────────────────────────────
        auto* w4 = new Worker("Sara Ahmed","sara@mofix.com",
                              "Pass@321","01098765432",
                              "10th of Ramadan","Deep Cleaning",200.0);
        w4->addReview(Review(w4->getId(),9999,"Nadia M.",4.8,"Best cleaning ever!"));

        auto* w10 = new Worker("Heba Mostafa","heba@mofix.com",
                               "Pass@444","01187654322",
                               "10th of Ramadan","Deep Cleaning",180.0);

        // ── AC Repair ─────────────────────────────────────
        auto* w5 = new Worker("Omar Farouk","omar@mofix.com",
                              "Pass@654","01187654321",
                              "10th of Ramadan","AC Repair",180.0);
        w5->addReview(Review(w5->getId(),9999,"Hassan A.",4.4,"Fixed it quickly."));

        auto* w11 = new Worker("Mahmoud Sayed","mahmoud@mofix.com",
                               "Pass@555","01276543211",
                               "10th of Ramadan","AC Repair",160.0);

        // ── Appliances ────────────────────────────────────
        auto* w6 = new Worker("Layla Nasser","layla@mofix.com",
                              "Pass@987","01276543210",
                              "10th of Ramadan","Appliances",130.0);
        w6->addReview(Review(w6->getId(),9999,"Dina F.",4.6,"Very efficient!"));

        auto* w12 = new Worker("Amr Wahid","amr@mofix.com",
                               "Pass@666","01009876543",
                               "10th of Ramadan","Appliances",120.0);

        workers.append(w1);  workers.append(w2);  workers.append(w3);
        workers.append(w4);  workers.append(w5);  workers.append(w6);
        workers.append(w7);  workers.append(w8);  workers.append(w9);
        workers.append(w10); workers.append(w11); workers.append(w12);
    }

public:
    Platform()
        : totalRevenue(0.0), platformFee(15.0), chatbot("MofBot") {
        seedWorkers();
    }
    ~Platform() {
        for (auto* c : customers)   delete c;
        for (auto* w : workers)     delete w;
        for (auto* b : allBookings) delete b;
    }
    Customer* registerCustomer(QString n,QString e,QString p,
                               QString ph,QString a) {
        auto* c = new Customer(n,e,p,ph,a);
        customers.append(c); return c;
    }
    Worker* registerWorker(QString n,QString e,QString p,QString ph,
                           QString a,QString cat,double price) {
        auto* w = new Worker(n,e,p,ph,a,cat,price);
        workers.append(w); return w;
    }
    User* login(const QString& email, const QString& pass) {
        for (auto* c : customers)
            if (c->getEmail()==email && c->checkPassword(pass)) return c;
        for (auto* w : workers)
            if (w->getEmail()==email && w->checkPassword(pass)) return w;
        return nullptr;
    }
    QVector<Worker*> getWorkersByCategory(const QString& cat) {
        QVector<Worker*> result;
        for (auto* w : workers)
            if (w->getCategory()==cat && w->getAvailable())
                result.append(w);
        std::sort(result.begin(), result.end(),
                  [](Worker* a, Worker* b){
                      return a->getRating() > b->getRating(); });
        return result;
    }
    Booking* createBooking(Customer* c, Worker* w,
                           const QString& desc,
                           const QString& slot, bool weekend) {
        double price = w->getPrice();
        if (weekend) price *= 0.8;
        Service svc(w->getCategory(), desc, w->getPrice());
        auto* b = new Booking(c->getId(), w->getId(),
                              w->getName(), svc, slot, price);
        allBookings.append(b);
        c->addBooking(b);
        w->addBooking(b);
        totalRevenue += price * (platformFee / 100.0);
        return b;
    }
    bool isWeekend() const {
        int d = QDate::currentDate().dayOfWeek();
        return d == 5 || d == 6;
    }
    QVector<Worker*>&    getWorkers()      { return workers; }
    QVector<Booking*>&   getAllBookings()   { return allBookings; }
    double               getTotalRevenue() const { return totalRevenue; }
    ChatBot&             getChatBot()      { return chatbot; }
    RecommendationEngine& getRecommender() { return recommender; }
};

// ============================================================
// MAINWINDOW
// ============================================================
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onLogout();
    void onBookNow();
    void onUpdateBookingStatus();
    void onToggleAvailability();
    void onTrackBooking();
    void onLeaveReview();
    void onSubmitReview();
    void onAIRecommendation();
    void onSaveProfile();
    void onSendChat();
    void onSearch();

private:
    Ui::MainWindow *ui;
    Platform        platform;
    QStackedWidget *stack;

    User*     currentUser    = nullptr;
    Worker*   selectedWorker = nullptr;
    QString   currentCategory;

    // ── Pages ──────────────────────────────────────────────
    QWidget *splashPage, *loginPage,     *registerPage;
    QWidget *customerHomePage,           *workerHomePage;
    QWidget *workerListPage,             *bookingPage;
    QWidget *myBookingsPage,             *trackPage;
    QWidget *reviewPage,                 *statusPage;
    QWidget *profilePage, *chatPage,     *recommendPage;

    // ── Login ──────────────────────────────────────────────
    QLineEdit *loginEmail, *loginPass;
    QLabel    *loginError;

    // ── Register ───────────────────────────────────────────
    QLineEdit *regName, *regEmail, *regPass, *regPhone, *regAddr;
    QComboBox *regType, *regCategory;
    QLabel    *regCategoryLbl;

    // ── Customer Home ──────────────────────────────────────
    QLabel    *custWelcome;
    QLineEdit *searchInput;
    QLabel    *searchMsg;

    // ── Worker List ────────────────────────────────────────
    QListWidget *workerListWidget;

    // ── Booking ────────────────────────────────────────────
    QLabel    *bookWorkerInfo, *bookPriceLabel, *bookDiscountLabel;
    QComboBox *bookTimeSlot;
    QLineEdit *bookDescription;

    // ── My Bookings ────────────────────────────────────────
    QListWidget *myBookingsList;

    // ── Track ──────────────────────────────────────────────
    QLabel *trackStatusLabel;

    // ── Review ─────────────────────────────────────────────
    QComboBox *reviewRating;
    QLineEdit *reviewComment;
    QLabel    *reviewMsg;

    // ── Worker Status ──────────────────────────────────────
    QListWidget *statusBookingList;
    QComboBox   *statusCombo;
    QLabel      *statusMsg;

    // ── Worker Home ────────────────────────────────────────
    QLabel *workerWelcome;

    // ── Profile ────────────────────────────────────────────
    QLabel    *profName, *profType, *profSaveMsg;
    QLineEdit *profPhone, *profAddr, *profPass;

    // ── Chat ───────────────────────────────────────────────
    QTextEdit  *chatDisplay;
    QLineEdit  *chatInput;

    // ── Recommend ──────────────────────────────────────────
    QLabel    *recommendLabel;
    QTextEdit *recommendDisplay;

    // ── Style ──────────────────────────────────────────────
    static const QString QSS;

    // ── Page builders ──────────────────────────────────────
    QWidget* buildSplashPage();
    QWidget* buildLoginPage();
    QWidget* buildRegisterPage();
    QWidget* buildCustomerHomePage();
    QWidget* buildWorkerHomePage();
    QWidget* buildWorkerListPage();
    QWidget* buildBookingPage();
    QWidget* buildMyBookingsPage();
    QWidget* buildTrackPage();
    QWidget* buildReviewPage();
    QWidget* buildStatusPage();
    QWidget* buildProfilePage();
    QWidget* buildChatPage();
    QWidget* buildRecommendPage();

    // ── Helpers ────────────────────────────────────────────
    void         goTo(QWidget* page);
    void         showWorkerList(const QString& category);
    void         selectWorker(int row);
    void         refreshMyBookings();
    void         refreshWorkerBookings();
    void         updateProfilePage();
    QPushButton* makeBtn(const QString& text,
                         const QString& obj,
                         QWidget* parent = nullptr);
    QScrollArea* makeScroll(QWidget*& inner);
};

#endif