void html_dst()
{
  THtml html;
  html.SetOutputDir("htmldoc/");
  html.SetSourceDir("../../T61DST/src/:../../T61DST/inc/");
  html.MakeAll(kFALSE,"T49*");
}
