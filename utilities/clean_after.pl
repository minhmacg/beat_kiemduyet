use strict;
use warnings;
use utf8;
use open qw(:std :utf8);
use feature 'say';
use Text::CSV;

my @plc = (
  "HÀNH VI PHẠM TỘI VÀ BẠO LỰC", "Bạo lực và khích nộ", "Cá nhân và tổ chức nguy hiểm",
  "Cấu kết gây hại và cổ xúy tội ác", "Hàng hóa và dịch vụ bị hạn chế", "Gian lận và lừa đảo",
  "AN TOÀN", "Tự tử, tự gây thương tích và chứng rối loạn ăn uống",
  "Ảnh khỏa thân, lạm dụng và bóc lột tình dục trẻ em", "Bóc lột tình dục người lớn",
  "Bắt nạt và quấy rối", "Bóc lột con người", "Vi phạm quyền riêng tư",
  "NỘI DUNG PHẢN CẢM", "Ngôn từ gây thù ghét", "Nội dung bạo lực và phản cảm",
  "Ảnh khỏa thân người lớn và hoạt động tình dục",
  "Hành vi gạ gẫm tình dục người lớn và ngôn ngữ khiêu dâm",
  "TÍNH TOÀN VẸN VÀ TÍNH XÁC THỰC", "Danh tính thực và tính toàn vẹn của tài khoản",
  "Spam", "An ninh mạng", "Hành vi gian dối", "Thông tin sai lệch", "Tưởng nhớ",
  "TÔN TRỌNG QUYỀN SỞ HỮU TRÍ TUỆ", "Quyền sở hữu trí tuệ",
  "An toàn và văn minh", "Hành vi bạo lực và hoạt động tội phạm",
  "Phát ngôn thù địch và hành vi thù địch", "Tổ chức và cá nhân bạo lực và thù địch",
  "Bóc lột và xâm hại trẻ vị thành niên", "Bóc lột tình dục và bạo lực trên cơ sở giới",
  "Bóc lột con người", "Quấy rối và bắt nạt", "Sức khỏe tâm thần và hành vi",
  "Tự tử và tự làm hại bản thân", "Rối loạn ăn uống và tự cảm nhận ngoại hình cơ thể",
  "Hoạt động và thử thách nguy hiểm", "Chủ đề nhạy cảm và dành cho người trưởng thành",
  "Hoạt động và dịch vụ tình dục", "Khỏa thân và phơi bày cơ thể", "Nội dung gợi dục",
  "Nội dung gây sốc và ghê rợn", "Ngược đãi động vật", "TÌNH TOÀN DIỆN VÀ CHÂN THỰC",
  "Thông tin sai lệch", "Tính liêm chính trong bầu cử và dân sự",
  "Nội dung đa phương tiện tổng hợp và bị thao túng", "Tương tác giả",
  "Nội dung không nguyên bản và mã QR", "Spam và hành vi lừa đảo bằng tài khoản",
  "HÀNG HÓA VÀ HOẠT ĐỘNG THƯƠNG MẠI BỊ KIỂM SOÁT", "Đánh bạc",
  "Rượu, thuốc lá và ma túy", "Súng và vũ khí nguy hiểm",
  "Buôn bán hàng hóa và dịch vụ bị kiểm soát", "Minh bạch thương mại và quảng cáo trả phí",
  "Gian lận và lừa đảo", "QUYỀN RIÊNG TƯ VÀ BẢO MẬT", "Thông tin cá nhân",
  "Bảo mật nền tảng", "Nguồn tin", "An ninh chính trị", "Tôn giáo", "Đối tác khách hàng",
);

my @stt = ("Duyệt", "Sửa/ xóa bài - cơ bản", "Sửa/ xóa bài - nâng cao");

# Open input file
my $file = shift or die "Usage: perl clean_after.pl <inputfile>\n";
open my $in, "<:encoding(UTF-8)", $file or die "Can't open file: $!";

# Create CSV parser for TSV
my $csv = Text::CSV->new({
  sep_char => "\t",
  binary => 1,
  auto_diag => 1,
  allow_loose_quotes => 1,
  allow_loose_escapes => 1,
}) or die "Cannot use CSV: " . Text::CSV->error_diag();

# Create CSV writer for output
my $csv_out = Text::CSV->new({
  sep_char => "\t",
  binary => 1,
  auto_diag => 1,
  eol => "\n",
}) or die "Cannot use CSV: " . Text::CSV->error_diag();

while (my $row = $csv->getline($in)) {
  my @fields = @$row;
  
  # Pad to 11 fields if needed
  push @fields, "" while @fields < 11;
  next if @fields > 11;

  # Clean time (remove after first space)
  $fields[0] =~ s/ .*$// if defined $fields[0];

  my $rstatus = defined $fields[8] ? $fields[8] : '';
  my $rplc    = defined $fields[10] ? $fields[10] : '';
  
  # Status mapping
  if ($rstatus =~ /^\d+$/) {
    $fields[8] = $stt[$rstatus - 1] // "Duyệt";
  } elsif ($rstatus eq '' || $rstatus eq 'L') {
    $fields[8] = "Duyệt";
  } else {
    warn "invalid status code $rstatus\n";
  }

  # Policy code mapping
  if ($rplc =~ /^[\d,]+$/) {
    my @codes = split /,/, $rplc;
    my @labels;
    for my $code (@codes) {
      if (defined $plc[$code]) {
        push @labels, "\"$plc[$code]\"";
      } else {
        warn "invalid plc index: $code\n";
      }
    }
    $fields[10] = join(",", @labels);
  }

  $csv_out->print(\*STDOUT, \@fields);
}

close $in;
