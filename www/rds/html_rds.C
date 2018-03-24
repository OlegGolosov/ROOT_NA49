void html_rds()
{
  THtml html;
  html.SetOutputDir("htmldoc/");
  html.SetSourceDir("../../src/:../../inc/");
  html.MakeAll(kFALSE,"TRootDS*");
}


