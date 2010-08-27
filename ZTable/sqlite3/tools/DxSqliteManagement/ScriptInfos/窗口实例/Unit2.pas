unit Unit2;

interface

uses
  SysUtils, Variants, Classes, Controls, Forms,
  Dialogs, StdCtrls, DB, ADODB, GridsEh, DBGridEh;

type
  TForm2 = class(TForm)
    Button1: TButton;
    ADOQuery1: TADOQuery;
    DBGridEh1: TDBGridEh;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

{$R *.dfm}

procedure TForm2.Button1Click(Sender: TObject);
begin

  ShowMessage('Hello');
end;

procedure TForm2.FormCreate(Sender: TObject);
begin
  ShowMessage('Created');
end;

end.
