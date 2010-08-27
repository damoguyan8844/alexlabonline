unit FrmExportXml;

interface

uses
  Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  ComCtrls, DB, MemDS, DBAccess, Uni, StdCtrls;

type
  TForm1 = class(TForm)
    ProgressBar1: TProgressBar;
    Button1: TButton;
    UniTable1: TUniTable;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

end.
