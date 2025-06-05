#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>// xử lý chữ in hoa , thường
#include<windows.h>
#include<string.h>
#include <time.h>
#define MAX_NAME 100
#define MAX_PHONE 100
#define MAX_CCCD 100
#define MAX_DATE 100
#define MAX_SERVICE 100
#define MAX_USER 100
#define MAX_PASSWORD 100

//===========================================================BACKEND===========================================================
//Hàm bổ trợ
char* layThoiGian() {
    static char date[11]; // Định dạng dd/mm/yyyy (10 ký tự + 1 ký tự null)
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);

    sprintf(date, "%02d/%02d/%04d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
    return date;
}
void sleep_fake(int n) {
    for (int i = 1;i <= n;i++) {
        for(long long j = 1;j <= 2000000000;j++) {
            //DELAY TIME
        }
    }
}
void eraseLines(int count) {// xóa count dòng
    if (count > 0) {
        printf("\x1b[2k");// xóa dòng hiện tại
        for (int i = 1; i < count; i++) {
            printf("\x1b[1A"); // Di chuyển con trỏ lên một dòng
            printf("\x1b[2K"); // Xóa nội dung trên dòng
        }
        printf("\r");
    }
}
int convert(char *s){ // biến chuỗi số thành số nguyên
    int result = 0;
    for(int i = 0; i < strlen(s); i++){
        result = result*10+((int)s[i]-48);
    }
    return result;
}
void UPCASE(char *s) // biến thành chuỗi in hoa
{
    for (size_t i = 0; s[i] != '\0'; ++i)
        s[i] = (char)toupper((unsigned char)s[i]);
}
//====================DỊCH VỤ====================
struct Node {
    int   maDichVu;
    char  tenDichVu[MAX_SERVICE];  /* lưu cả chuỗi tên */
    double giaTien;
    struct Node *next;
};

typedef Node* PNode;
typedef Node* DanhSachDV;

void initListDV(DanhSachDV &list)        { list = NULL; }
int isEmpty(DanhSachDV  list)         { return list == NULL; }

void themDV(DanhSachDV &list, int id, const char *name, double price)
{
    PNode q = (PNode)malloc(sizeof(Node));
    q->maDichVu = id;

    strncpy(q->tenDichVu, name, MAX_SERVICE);
    q->tenDichVu[MAX_SERVICE - 1] = '\0';

    q->giaTien = price;

    q->next = list;
    list    = q;
}

PNode timKiemDV(DanhSachDV list, int k)
{
    for (PNode p = list; p != NULL; p = p->next)
        if (p->maDichVu == k) return p;
    return NULL;
}

PNode timKiemDVTen(DanhSachDV list, const char *keyword)
{
    if (list == NULL || keyword == NULL) return NULL;
    char keyUp[MAX_SERVICE];
    strncpy(keyUp, keyword, MAX_SERVICE);
    keyUp[MAX_SERVICE-1] = '\0';
    UPCASE(keyUp);
    for (PNode p = list; p != NULL; p = p->next)
    {
        char nameUp[MAX_SERVICE];
        strncpy(nameUp, p->tenDichVu, MAX_SERVICE);
        nameUp[MAX_SERVICE-1] = '\0';
        UPCASE(nameUp);

        if (strstr(nameUp, keyUp) != NULL)
            return p;
    }
    return NULL;
}

void suaDV(PNode q, const char *name, double price)
{
    if (q == NULL) return;

    strncpy(q->tenDichVu, name, MAX_SERVICE);
    q->tenDichVu[MAX_SERVICE - 1] = '\0';
    q->giaTien = price;
}

void xoaDV(DanhSachDV &list, PNode p)
{
    if (isEmpty(list) || p == NULL) return;

    if (p == list)
        list = list->next;
    else {
        PNode prev = list;
        while (prev && prev->next != p) prev = prev->next;
        if (!prev) return;
        prev->next = p->next;
    }
    free(p);
}

void hienThiDV(DanhSachDV list)
{
    printf("--------------------------------------------------\n");
    for (PNode p = list; p != NULL; p = p->next)
        printf("Ma: %-5d | Ten: %-25s | Gia: %.0lf VND\n",
               p->maDichVu, p->tenDichVu, p->giaTien);
    printf("--------------------------------------------------\n");
}
//====================BỆNH NHÂN====================
//1. Định nghĩa danh sách liên kết đơn và hàng đợi
struct NodeBN {
    int   STT;                                 // số thứ tự
    char  tenBenhNhan[MAX_NAME];               // tên bệnh nhân (chuỗi)
    int   tuoiBenhNhan;
    char  SDT[MAX_PHONE];                      // số điện thoại
    char  CCCD[MAX_CCCD];                      // căn cước
    char  tinhTrangBenh[MAX_NAME];             // mô tả bệnh
    char  lichKham[MAX_DATE];                  // ngày khám (dd/mm/yyyy)
    int   BHYT;                                // 1:có, 0:không
    Node *dichVu;                            // dịch vụ đã chọn
    struct NodeBN *next;                       // node kế tiếp
};
typedef NodeBN* TNode;
typedef NodeBN* DanhSachBN;
struct QueueBN {
    TNode front;
    TNode rear;
};
//2. Hàng đợi cho bệnh nhân
void initQueue(QueueBN& q) {
    q.front = q.rear = NULL;
}
bool isQueueEmpty(QueueBN q) {
    return (q.front == NULL);
}
void enqueueBN(QueueBN& q, TNode newBN) {
    newBN->next = NULL;
    if (isQueueEmpty(q)) {
        q.front = q.rear = newBN;
    } else {
        q.rear->next = newBN;
        q.rear = newBN;
    }
}
TNode dequeueBN(QueueBN& q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }
    TNode temp = q.front;
    q.front = q.front->next;
    if (q.front == NULL) {
        q.rear = NULL;
    }
    temp->next = NULL; // tách khỏi hàng đợi
    return temp;
}
TNode peekQueue(const QueueBN &q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }
    return q.front;
}
QueueBN hangDoi;
//3. Danh sách liên kết đơn cho bệnh nhân
void initListBN(DanhSachBN& list) {
    list = NULL;
}
int isEmpty(DanhSachBN list) {
    return (list == NULL);
}
void themBN(DanhSachBN &list,
            int   STT2,
            const char *tenBenhNhan2,
            int   tuoiBenhNhan2,
            const char *SDT2,
            const char *CCCD2,
            const char *tinhTrangBenh2,
            const char *lichKham2,
            int   BHYT2,
            Node *dichVu2)
{
    TNode q = (TNode)malloc(sizeof(NodeBN));

    q->STT          = STT2;
    strncpy(q->tenBenhNhan, tenBenhNhan2, MAX_NAME);
    q->tenBenhNhan[MAX_NAME-1] = '\0';

    q->tuoiBenhNhan = tuoiBenhNhan2;

    strncpy(q->SDT,  SDT2,  MAX_PHONE);
    q->SDT[MAX_PHONE-1] = '\0';

    strncpy(q->CCCD, CCCD2, MAX_CCCD);
    q->CCCD[MAX_CCCD-1] = '\0';

    strncpy(q->tinhTrangBenh, tinhTrangBenh2, MAX_NAME);
    q->tinhTrangBenh[MAX_NAME-1] = '\0';

    strncpy(q->lichKham, lichKham2, MAX_DATE);
    q->lichKham[MAX_DATE-1] = '\0';

    q->BHYT  = BHYT2;
    q->dichVu = dichVu2;
    q->next = list;
    list    = q;
    enqueueBN(hangDoi, q);
}
TNode timKiemBNTen(DanhSachBN list, const char *name)
{
    char key[MAX_NAME];
    strncpy(key, name, MAX_NAME);
    key[MAX_NAME-1] = '\0';
    UPCASE(key);

    for (TNode p = list; p; p = p->next) {
        char tmp[MAX_NAME];
        strncpy(tmp, p->tenBenhNhan, MAX_NAME);
        tmp[MAX_NAME-1] = '\0';
        UPCASE(tmp);

        if (strcmp(tmp, key) == 0)
            return p;
    }
    return NULL;
}
TNode timKiemBNSDT(DanhSachBN list, const char *SDTtemp)
{
    for (TNode ptr = list; ptr != NULL; ptr = ptr->next)
        if (strcmp(ptr->SDT, SDTtemp) == 0)
            return ptr;
    return NULL;
}
void suaBN(DanhSachBN &list,
           TNode       q,
           int         STT2,
           const char *tenBenhNhan2,
           int         tuoiBenhNhan2,
           const char *SDT2,
           const char *CCCD2,
           const char *tinhTrangBenh2,
           const char *lichKham2,
           int         BHYT2,
           Node     *dichVu2)
{
    if (isEmpty(list) || q == NULL)  return;

    q->STT = STT2;

    strncpy(q->tenBenhNhan, tenBenhNhan2, MAX_NAME);
    q->tenBenhNhan[MAX_NAME - 1] = '\0';

    q->tuoiBenhNhan = tuoiBenhNhan2;

    strncpy(q->SDT, SDT2, MAX_PHONE);
    q->SDT[MAX_PHONE - 1] = '\0';

    strncpy(q->CCCD, CCCD2, MAX_CCCD);
    q->CCCD[MAX_CCCD - 1] = '\0';

    strncpy(q->tinhTrangBenh, tinhTrangBenh2, MAX_NAME);
    q->tinhTrangBenh[MAX_NAME - 1] = '\0';

    strncpy(q->lichKham, lichKham2, MAX_DATE);
    q->lichKham[MAX_DATE - 1] = '\0';

    q->BHYT   = BHYT2;
    q->dichVu = dichVu2;
}
void xoaBN(DanhSachBN& list, NodeBN* p) {
    if (isEmpty(list)) {
        return;
    }
    if (p == NULL) {
        return;
    }

    if (p == list) {
        list = list->next;
        delete p;
        return;
    }
    NodeBN* prev = list;
    while (prev != NULL && prev->next != p) {
        prev = prev->next;
    }

    if (prev == NULL) {
        return;
    }
    prev->next = p->next;
    delete p;
}
void hienThiMotBN(DanhSachBN& list, TNode q) {
    printf("--------------------------------------------------\n");
    if (isEmpty(list) || q == NULL)  printf("Danh sach rong\n");
    printf("STT : %d\n",q->STT);
    printf("Ten benh nhan : %s\n",q->tenBenhNhan);
    printf("Tuoi benh nhan : %d\n",q->tuoiBenhNhan);
    printf("SDT : %s\n",q->SDT);
    printf("CCCD : %s\n",q->CCCD);
    printf("Tinh trang benh : %s\n",q->tinhTrangBenh);
    printf("Lich kham : %s\n",q->lichKham);
    printf("Dich vu : %s\n",q->dichVu->tenDichVu);
    if (q->BHYT) printf("BHYT : Co\n");
    else printf("BHYT : Khong\n");
    printf("--------------------------------------------------\n");
}
void hienThiBN(DanhSachBN& list) {
    for (TNode q = list; q != NULL; q = q->next) {
        printf("--------------------------------------------------\n");
        if (isEmpty(list) || q == NULL){
            printf("Danh sach rong\n");
            return;
        }
        printf("STT : %d\n",q->STT);
        printf("Ten benh nhan : %s\n",q->tenBenhNhan);
        printf("Tuoi benh nhan : %d\n",q->tuoiBenhNhan);
        printf("SDT : %s\n",q->SDT);
        printf("CCCD : %s\n",q->CCCD);
        printf("Tinh trang benh : %s\n",q->tinhTrangBenh);
        printf("Lich kham : %s\n",q->lichKham);
        printf("Dich vu : %s\n",q->dichVu->tenDichVu);
        if (q->BHYT) printf("BHYT : Co\n");
        else printf("BHYT : Khong\n");
        printf("--------------------------------------------------\n");
    }
}
//====================HÓA ĐƠN====================
struct QueueHD {
    TNode front;
    TNode rear;
};
void initQueueHD(QueueHD& q) {
    q.front = q.rear = NULL;
}
bool isQueueHDEmpty(QueueHD q) {
    return (q.front == NULL);
}
void enqueueHD(QueueHD& q, TNode newHD) {
    newHD->next = NULL;
    if (isQueueHDEmpty(q)) {
        q.front = q.rear = newHD;
    } else {
        q.rear->next = newHD;
        q.rear = newHD;
    }
}
TNode dequeueHD(QueueHD& q) {
    if (isQueueHDEmpty(q)) {
        return NULL;
    }
    TNode temp = q.front;
    q.front = q.front->next;
    if (q.front == NULL) {
        q.rear = NULL;
    }
    temp->next = NULL; // tách khỏi hàng đợi
    return temp;
}
TNode peekQueueHD(const QueueHD &q) {
    if (isQueueHDEmpty(q)) {
        return NULL;
    }
    return q.front;
}
void hienThiHD(DanhSachBN& list, TNode q) {
    if (isEmpty(list) || q == NULL)  {
        printf("Khong co hoa don nao !\n");
        return;
    }
    printf("--------------------------------------------------\n");
    printf("STT : %d\n",q->STT);
    printf("Ten benh nhan : %s\n",q->tenBenhNhan);
    printf("Tuoi benh nhan : %d\n",q->tuoiBenhNhan);
    printf("SDT : %s\n",q->SDT);
    printf("CCCD : %s\n",q->CCCD);
    printf("Tinh trang benh : %s\n",q->tinhTrangBenh);
    printf("Lich kham : %s\n",q->lichKham);
    printf("Dich vu : %s\n",q->dichVu->tenDichVu);
    if (q->BHYT) printf("BHYT : Co\n");
    else printf("BHYT : Khong\n");
    printf("--------------------------------------------------\n");
    printf("Gia dich vu : %lf VND\n", q->dichVu->giaTien);
    if (q->BHYT) printf("Chiet khau BHYT : 50%% \n");
    printf("--------------------------------------------------\n");
    printf("TONG : %lf VND \n", (q->dichVu->giaTien) / ( q->BHYT ? 2.0 : 1.0 ));
}
QueueHD hangDoiHoaDon;