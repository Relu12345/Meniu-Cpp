#include <iostream>
#include <cstring>
#include <cstdio>
#include <windows.h>
#include <ctime>

using namespace std;

class Vaccin
{
    int tip_vaccin;
    char data[256];
    int centru;
    public:
        void setV(int v){tip_vaccin=v;}
        void setD(char d[256]){strcpy(data, d);}
        void setC(int c){centru=c;}
        int getV(){return tip_vaccin;}
        char* getD(){return data;}
        int getC(){return centru;}
        friend ostream &operator<<(ostream&, const Vaccin&);
};

void SetColor(int val)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  val);
}

ostream &operator<<(ostream& os, const Vaccin &vac)
{
    SetColor(6);
    os << "|\n";
    SetColor(1);
    os << "Tip vaccin: ";
    SetColor(7);
    if(vac.tip_vaccin==1)
    {
        os << "Anti-Covid\t\t\t      ";
        SetColor(6);
        os << "|\n";
        SetColor(1);
    }
    else
    {
        os << "Antigripal\t\t\t      ";
        SetColor(6);
        os << "|\n";
        SetColor(1);
    }
    os << "Data: ";
    SetColor(7);
    os << vac.data << "\t\t\t      ";
    SetColor(6);
    os << "|\n";
    SetColor(1);
    os << "Centru: ";
    SetColor(7);
    os << "Optiunea " << vac.centru << "\t\t\t      ";
    SetColor(6);
    os << "|\n\t\t\t\t\t      |\n";
    return os;
}

int len(int n)
{
    int k=0;
    while(n)
        k++, n/=10;
    return k;
}

int bisect(int an)
{
    if(an%400==0)
        return 1;
    if(an%100==0)
        return 0;
    if(an%4==0)
        return 1;
    return 0;
}
int checkData(char* zi, char* luna, char* an)
{
    time_t timp = time(NULL);
    struct tm *aTimp = localtime(&timp);
    int zi_ver = aTimp->tm_mday;
    int luna_ver = aTimp->tm_mon + 1;
    int an_ver = aTimp->tm_year + 1900;
    if(atoi(an)<an_ver)
        return 0;
    if(atoi(an)==an_ver)
    {
        if(atoi(luna)<luna_ver)
        {
            return 0;
        }
        if(atoi(luna)==luna_ver)
        {
            if(atoi(zi)<zi_ver)
            {
                return 0;
            }
        }
    }
    if(strlen(an)!=4)
        return 0;
    if(bisect(atoi(an))==1)
    {
        if(atoi(luna)==2)
            if(atoi(zi)>29)
                return 0;
    }
    else
    {
        if(atoi(luna)==2)
            if(atoi(zi)>28)
                return 0;
    }
    if(strlen(luna)!=2)
        return 0;
    if(atoi(luna)<1 || atoi(luna)>12)
        return 0;
    if(atoi(luna)<8)
    {
        if(atoi(luna)%2)
        {
            if(atoi(zi)>31)
                return 0;
        }
        else
        {
            if(atoi(zi)>30)
                return 0;
        }
    }
    else
    {
        if(atoi(luna)%2==0)
        {
            if(atoi(zi)>31)
               return 0;
        }
        else
        {
            if(atoi(zi)>30)
                return 0;
        }
    }
    if(strlen(zi)!=2)
        return 0;
    if(atoi(zi)<1 || atoi(zi)>31)
        return 0;
    return 1;
}

void stergere_fisier(int id, int op, int ok)
{
    FILE *db=fopen("db.txt", "r+");
    FILE *db2=fopen("db2.txt", "w");
    int i;
    char c[256];
    op--;
    for(i=1;i<=id;i++)
    {
        if(i==op+1 && ok)
        {
            fgets(c, 256, db);
            fgets(c, 256, db);
            fgets(c, 256, db);
            fgets(c, 256, db);
        }
        else
        {
            fgets(c, 256, db);
            c[strlen(c)-1]='\0';
            fprintf(db2, "%s\n", c);
            fgets(c, 256, db);
            c[strlen(c)-1]='\0';
            fprintf(db2, "%s\n", c);
            fgets(c, 256, db);
            c[strlen(c)-1]='\0';
            fprintf(db2, "%s\n\n", c);
            fgets(c, 256, db);
        }
    }
    fclose(db);
    fclose(db2);
    remove("db.txt");
    rename("db2.txt", "db.txt");
}

int fisier_id(int id)
{
    FILE *db=fopen("db.txt", "r");
    fseek(db, -1, SEEK_END);
    char c[256];
    int n;
    fscanf(db, "%s", c);
    n=atoi(c);
    fseek(db, -n-3, SEEK_END);
    fscanf(db, "%[^\n]", c);
    id=atoi(c);
    fclose(db);
    return id;
}

int main()
{
    FILE *f=fopen("db.txt", "r+");
    Vaccin v[105];
    int id=0, id_prec, optiune, i, init=0, ok=0, ok_c, k_caut_grip=0, k_caut_cov=0, k_fin;
    if(f==NULL)
    {
        fclose(f);
        f=fopen("db.txt", "w");
        fclose(f);
        f=fopen("db.txt", "r+");
    }
    id=fisier_id(id);
    id_prec=id;
    char c[256];
    for(i=1;i<=id;i++)
    {
        fgets(c, 256, f);
        c[strlen(c)-1]='\0';
        v[i].setV(atoi(c));
        if(atoi(c)==1){k_caut_cov++;}
        else if(atoi(c)==2){k_caut_grip++;}
        fgets(c, 256, f);
        c[strlen(c)-1]='\0';
        v[i].setD(c);
        fgets(c, 256, f);
        c[strlen(c)-1]='\0';
        v[i].setC(atoi(c));
        fgets(c, 256, f);
    }
    fseek(f, -1, SEEK_END);
    int n;
    fscanf(f, "%s", c);
    n=atoi(c);
    start:
    fseek(f, -n-3, SEEK_END);
    SetColor(7);
    system("cls");
    cout << "\t\t\t\t       Program vaccinare\n";
    SetColor(6);
    cout << "--------------------------------------------------------\n\n";
    SetColor(7);
    cout << "Alege o optiune:";
    SetColor(6);
    cout << "\n\n-------------------------------------\n";
    SetColor(10);
    cout << "1) ";
    SetColor(7);
    cout << "Rezervare pentru vaccin\t    ";
    SetColor(6);
    cout << "|\n";
    SetColor(10);
    cout << "2) ";
    SetColor(7);
    cout << "Vizualizeaza o rezervare\t    ";
    SetColor(6);
    cout << "|\n";
    SetColor(10);
    cout << "3) ";
    SetColor(7);
    cout << "Anuleaza o rezervare\t\t    ";
    SetColor(6);
    cout << "|\n";
    SetColor(10);
    cout << "4) ";
    SetColor(7);
    cout << "Cautare rezervare vaccin\t    ";
    SetColor(6);
    cout << "|\n";
    SetColor(10);
    cout << "5) ";
    SetColor(7);
    cout << "Iesire program\t\t    ";
    SetColor(6);
    cout << "|\n-------------------------------------\n\n--------------------------------------------------------\n";
    SetColor(7);
    int tip_vac, centrul, nrSl, optiune_id, n_final;
    char dat[256], tmp[256], zi[3], luna[3], an[5], *p;
    ok_c=0;
    cin >> optiune;
    switch(optiune)
    {
        case 1:
        {
            system("cls");
            goto_tip_vaccin:
            cout << "\t\t\t\t       Program vaccinare\n";
            SetColor(6);
            cout << "--------------------------------------------------------\n\n";
            SetColor(7);
            cout << "Alege tipul de vaccin:\n\n";
            SetColor(6);
            cout << "-------------------------------------\n";
            SetColor(10);
            cout << "1) ";
            SetColor(7);
            cout << "Vaccin Anti-Covid\t\t    ";
            SetColor(6);
            cout << "|\n";
            SetColor(10);
            cout << "2) ";
            SetColor(7);
            cout << "Vaccin Antigripal\t\t    ";
            SetColor(6);
            cout << "|\n";
            SetColor(10);
            cout << "3) ";
            SetColor(7);
            cout << "Inapoi la meniu\t\t    ";
            SetColor(6);
            cout << "|\n-------------------------------------\n\n--------------------------------------------------------\n";
            SetColor(7);
            id_prec=id;
            id++;
            cin >> tip_vac;
            switch(tip_vac)
            {
                case 1:
                {
                    k_caut_cov++;
                    goto_programare:
                    getchar();
                    system("cls");
                    cout << "\t\t\t\t       Program vaccinare\n";
                    SetColor(6);
                    cout << "--------------------------------------------------------\n\n";
                    SetColor(7);
                    cout << "Introdu data programarii";
                    SetColor(2);
                    cout << "(sau introdu \"0\" pentru a reveni la meniu)";
                    SetColor(7);
                    cout << ":\n";
                    SetColor(6);
                    cout << "Format: DD/MM/YYYY\n\n";
                    SetColor(7);
                    cin.getline(dat, 256);
                    strcpy(tmp, dat);
                    nrSl=0;
                    for(size_t i=0;i<strlen(tmp);i++)
                    {
                        if(tmp[i]=='/')
                            nrSl++;
                    }
                    if(strcmp(tmp, "0")==0)
                    {
                        id--;
                        goto start;
                    }
                    if(nrSl!=2)
                    {
                        SetColor(4);
                        cout << "[Format data incorect!]\n";
                        SetColor(7);
                        cout << "Apasati \"Enter\" ca sa reincercati\n";
                        goto goto_programare;
                    }
                    p=strtok(tmp, "/");
                    strcpy(zi, p);
                    p=strtok(NULL, "/");
                    strcpy(luna, p);
                    p=strtok(NULL, "/");
                    strcpy(an, p);
                    if(!checkData(zi, luna, an))
                    {
                        SetColor(4);
                        cout << "[Data incorecta!]\n";
                        SetColor(7);
                        cout << "Apasati \"Enter\" ca sa reincercati\n";
                        goto goto_programare;
                    }
                    cout << "\n\n";
                    goto_centru:
                    system("cls");
                    cout << "\t\t\t\t       Program vaccinare\n";
                    SetColor(6);
                    cout << "--------------------------------------------------------\n\n";
                    SetColor(7);
                    cout << "Alege centrul de vaccinare";
                    SetColor(2);
                    cout << "(sau introdu \"0\" pentru a reveni la meniu)";
                    SetColor(7);
                    cout << ":\n\n";
                    SetColor(6);
                    cout << "-------------------------------------\n";
                    SetColor(10);
                    cout << "1) ";
                    SetColor(7);
                    cout << "Optiune 1\t\t\t    ";
                    SetColor(6);
                    cout << "|\n";
                    SetColor(10);
                    cout << "2) ";
                    SetColor(7);
                    cout << "Optiune 2\t\t\t    ";
                    SetColor(6);
                    cout << "|\n";
                    SetColor(10);
                    cout << "3) ";
                    SetColor(7);
                    cout << "Optiune 3\t\t\t    ";
                    SetColor(6);
                    cout << "|\n";
                    SetColor(10);
                    cout << "4) ";
                    SetColor(7);
                    cout << "Optiune 4\t\t\t    ";
                    SetColor(6);
                    cout << "|\n";
                    SetColor(10);
                    cout << "5) ";
                    SetColor(7);
                    cout << "Optiune 5\t\t\t    ";
                    SetColor(6);
                    cout << "|\n";
                    SetColor(10);
                    cout << "6) ";
                    SetColor(7);
                    cout << "Optiune 6\t\t\t    ";
                    SetColor(6);
                    cout << "|\n-------------------------------------\n\n--------------------------------------------------------\n";
                    SetColor(7);
                    cin >> centrul;
                    if(!centrul)
                    {
                        id--;
                        goto start;
                    }
                    v[id].setV(tip_vac);
                    v[id].setD(dat);
                    v[id].setC(centrul);
                    switch(centrul)
                    {
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        {
                            system("cls");
                            cout << "\t\t\t\t       Program vaccinare\n";
                            SetColor(6);
                            cout << "--------------------------------------------------------\n\n";
                            SetColor(10);
                            cout << "[Programare finalizata]\n\n";
                            SetColor(7);
                            cout << "Date programare:\n";
                            SetColor(6);
                            cout << "----------------------------------------\n";
                            SetColor(1);
                            cout << "Tip vaccin: ";
                            SetColor(7);
                            if(v[id].getV()==1)
                                cout << "Anti-Covid";
                            else
                                cout << "Antigripal";
                            if(id_prec && init)
                                id_prec=id, fprintf(f, "\n\n");
                            ok=1;
                            init=1;
                            fprintf(f, "%d\n", v[id].getV());
                            fflush(f);
                            fprintf(f, "%s\n%d\n\n", v[id].getD(), v[id].getC());
                            fflush(f);
                            cout << "\t\t       ";
                            SetColor(6);
                            cout << "|\n";
                            SetColor(1);
                            cout << "Data Programarii: ";
                            SetColor(7);
                            cout << v[id].getD() << "\t       ";
                            SetColor(6);
                            cout << "|\n";
                            SetColor(1);
                            cout << "Centru Vaccinare: ";
                            SetColor(7);
                            cout << "Optiunea " << v[id].getC() << "\t       ";
                            SetColor(6);
                            cout << "|\n----------------------------------------\n\n";
                            SetColor(7);
                            system("pause");
                            goto start;
                        }
                        default:
                            goto goto_centru;
                    }
                    goto start;
                    break;
                }
                case 2:
                {
                    k_caut_grip++;
                    system("cls");
                    goto goto_programare;
                    break;
                }
                case 3:
                {
                    system("cls");
                    id--;
                    goto start;
                    break;
                }
                default:
                {
                    system("cls");
                    goto goto_tip_vaccin;
                }
            }
            break;
        }
        case 2:
        {
            system("cls");
            if(id==0)
            {
                SetColor(4);
                cout << "[Nu exista rezervari!]\n";
                SetColor(7);
                system("pause");
                goto start;
            }
            cout << "\t\t\t\t       Program vaccinare\n";
            SetColor(6);
            cout << "--------------------------------------------------------\n\n";
            SetColor(10);
            cout << "Apasati orice buton ca sa reveniti la meniul principal\n\n";
            SetColor(6);
            cout << "-----------------------------------------------\n\t\t\t\t\t      |\n";
            for(int i=1;i<=id;i++)
            {
                SetColor(1);
                cout << "ID: ";
                SetColor(7);
                cout << i << "\t\t\t\t\t      ";
                cout << v[i];
                if(i==id)
                    cout << "-----------------------------------------------\n\n";
                else
                    cout << "----------------------------------------------|\n\t\t\t\t\t      |\n";
            }
            cout << "--------------------------------------------------------\n\n";
            SetColor(7);
            system("pause");
            goto start;
        }
        case 3:
        {
            goto_anulare:
            system("cls");
            if(id==0)
            {
                SetColor(4);
                cout << "[Nu exista rezervari!]\n";
                SetColor(7);
                system("pause");
                goto start;
            }
            cout << "\t\t\t\t       Program vaccinare\n";
            SetColor(6);
            cout << "--------------------------------------------------------\n\n";
            SetColor(7);
            cout << "Pentru a anula o rezervare, introduceti ID-ul rezervarii\n";
            SetColor(10);
            cout << "Introduceti \"0\" ca sa reveniti la meniul principal\n\n";
            SetColor(6);
            cout << "-----------------------------------------------\n\t\t\t\t\t      |\n";
            for(int i=1;i<=id;i++)
            {
                SetColor(1);
                cout << "ID: ";
                SetColor(7);
                cout << i << "\t\t\t\t\t      ";
                cout << v[i];
                if(i==id)
                    cout << "-----------------------------------------------\n\n";
                else
                    cout << "----------------------------------------------|\n\t\t\t\t\t      |\n";
            }
            cout << "--------------------------------------------------------\n\n";
            SetColor(10);
            cout << "ID-ul rezervarii: ";
            SetColor(7);
            cin >> optiune_id;
            if(optiune_id==0)
                goto start;
            if(optiune_id>id)
            {
                system("cls");
                SetColor(4);
                cout << "[Nu exista rezervarea cu ID-ul respectiv]\n";
                SetColor(7);
                system("pause");
                goto goto_anulare;
            }
            for(int i=optiune_id;i<id;i++)
                v[i]=v[i+1];
            fclose(f);
            ok=1;
            init=1;
            stergere_fisier(id, optiune_id, 1);
            f=fopen("db.txt", "r+");
            id--;
            id_prec=id;
            system("cls");
            SetColor(10);
            cout << "[Rezervare anulata cu succes]\n";
            SetColor(7);
            system("pause");
            goto start;
        }
        case 4:
        {
            goto_cautare:
            system("cls");
            if(id==0)
            {
                SetColor(4);
                cout << "[Nu exista rezervari!]\n";
                SetColor(7);
                system("pause");
                goto start;
            }
            cout << "\t\t\t\t       Program vaccinare\n";
            SetColor(6);
            cout << "--------------------------------------------------------\n\n";
            SetColor(7);
            cout << "Cautati un vaccin \n";
            SetColor(10);
            cout << "Introduceti \"Anti-Covid\" sau \"Antigripal\" pentru a cauta rezervari cu vaccinul respectiv\nSau introduceti \"0\" pentru a reveni la meniu)\n\n";
            SetColor(7);
            string cautare;
            if(!ok_c)
                cin.ignore();
            getline(cin, cautare);
            if((cautare.compare("Anti-Covid")==0 && k_caut_cov==0) || (cautare.compare("Antigripal")==0 && k_caut_grip==0))
            {
                SetColor(4);
                cout << "[Nu exista rezervari de acest tip!]\n";
                SetColor(7);
                system("pause");
                goto start;
            }
            SetColor(6);
            cout << "-----------------------------------------------\n\t\t\t\t\t      |\n";
            SetColor(1);
            k_fin=0;
            for(int i=1;i<=id;i++)
            {
                if(cautare.compare("Anti-Covid")==0)
                {
                    if(v[i].getV()==1)
                    {
                        k_fin++;
                        SetColor(1);
                        cout << "ID: ";
                        SetColor(7);
                        cout << i << "\t\t\t\t\t      ";
                        cout << v[i];
                        if(k_fin==k_caut_cov)
                            cout << "-----------------------------------------------\n\n";
                        else
                            cout << "----------------------------------------------|\n\t\t\t\t\t      |\n";
                    }
                }
                else if(cautare.compare("Antigripal")==0)
                {
                    if(v[i].getV()==2)
                    {
                        k_fin++;
                        SetColor(1);
                        cout << "ID: ";
                        SetColor(7);
                        cout << i << "\t\t\t\t\t      ";
                        cout << v[i];
                        if(k_fin==k_caut_grip)
                            cout << "-----------------------------------------------\n\n";
                        else
                            cout << "----------------------------------------------|\n\t\t\t\t\t      |\n";
                    }
                }
                else if(cautare.compare("0")==0)
                {
                    goto start;
                }
                else
                {
                    system("cls");
                    SetColor(4);
                    cout << "[Optiunea aleasa nu exista!]\n";
                    SetColor(7);
                    cout << "Apasati \"Enter\" ca sa reincercati\n";
                    cin.ignore();
                    ok_c++;
                    goto goto_cautare;
                }
            }
            SetColor(6);
            cout << "--------------------------------------------------------\n\n";
            SetColor(7);
            system("pause");
            system("cls");
            goto start;
        }
        case 5:
        {
            system("cls");
            SetColor(6);
            cout << "----------------\n  ";
            SetColor(10);
            cout << "La revedere  ";
            SetColor(6);
            cout << "|\n----------------";
            SetColor(7);
            n_final=len(id);
            if(ok && id)
            {
                fseek(f, -n-3, SEEK_END);
                fprintf(f, "\n\n%d\n", id);
                fprintf(f, "%d", n_final);
                fflush(f);
            }
            fclose(f);
            break;
        }
        default:
        {
            system("cls");
            SetColor(4);
            cout << "[Optiunea selectata nu este valida!]\n";
            SetColor(7);
            system("pause");
            goto start;
        }
     }
}
