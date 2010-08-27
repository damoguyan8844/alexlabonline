unit Unit1;

interface

uses
  Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, DB, GridsEh, DBGridEh, MemDS, DBAccess, Uni;

type
  TForm1 = class(TForm)
    Button1: TButton;
    UniTable1: TUniTable;
    DBGridEh1: TDBGridEh;
    DataSource1: TDataSource;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Button1Click(Sender: TObject);
begin
  ShowMessage(UniTable1.TableName);
end;

end.
