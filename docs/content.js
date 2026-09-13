window.MATRIXFLASH_DOCS = [
  {
    id: '01', group: 'Başlangıç', title: 'MatrixFlash ile tanış', slug: 'tanisma', summary: 'Kütüphanenin amacını, tasarım kararlarını ve bu rehberin nasıl kullanılacağını kavra.', why: 'MatrixFlash hazır bir black box değil; matris işlemlerinin bellekte ve matematikte nasıl çalıştığını görünür kılmak için tasarlandı.', math: 'Her matris, satır ve sütunlarla düzenlenmiş sayılar topluluğudur. Kütüphanedeki bütün ileri işlemler bu küçük veri modelinin üzerine inşa edilir.', code: '#include "Matrix/Matrix.hpp"\n\nint main() {\n    Matrix input(2, 3);\n    input.at(0, 0) = 1.0;\n    return 0;\n}', note: 'Bu dokümantasyonda önce veri modelini, sonra işlemleri, en son da yapay zeka akışını öğreneceksin.'
  },
  {
    id: '02', group: 'Başlangıç', title: 'Proje yapısını oku', slug: 'proje-yapisi', summary: 'include, src, tests ve examples klasörlerinin sorumluluklarını ayır.', why: 'İyi bir kütüphaneyi kullanmak kadar, davranışın hangi dosyada yaşadığını bulabilmek de önemlidir.', math: 'Public API, kullanıcı ile implementasyon arasındaki sözleşmedir. Header bildirir; kaynak dosyası hesaplar.', code: 'include/Matrix/Matrix.hpp  // public sözleşme\nsrc/MatrixArithmetic.cpp   // aritmetik\nsrc/MatrixActivation.cpp   // ReLU, Softmax\ntests/MatrixTests.cpp       // davranış testleri', note: 'Yeni bir özellikte sıralama: header bildirimi, ilgili cpp implementasyonu, test ve README açıklaması.'
  },
  {
    id: '03', group: 'Matris temelleri', title: 'Matris nedir?', slug: 'matris-nedir', summary: 'Matrisin tablo, dönüşüm ve veri kabı olarak üç farklı rolünü öğren.', why: 'Yapay zeka modellerindeki ağırlıklar, girdiler ve aktivasyonlar çoğunlukla matris olarak taşınır.', math: 'A = [aᵢⱼ] gösteriminde i satırı, j sütunu ifade eder. 2×3 matrisin 2 satırı ve 3 sütunu, toplam 6 hücresi vardır.', code: 'Matrix A(2, 3);\n// [ a00 a01 a02 ]\n// [ a10 a11 a12 ]', note: 'Matris boyutu yalnızca şekil değildir; hangi işlemlerin geçerli olduğunu da belirler.'
  },
  {
    id: '04', group: 'Matris temelleri', title: 'Boyut, satır ve sütun', slug: 'boyutlar', summary: 'm×n gösterimini okuyup işlemlerin boyutlarını önceden tahmin et.', why: 'Boyutları işlemden önce kontrol etmek, hatayı derin bir döngüde değil çağrı noktasında yakalar.', math: '(m×n) + (m×n) geçerlidir. (m×n)·(n×p) sonucu (m×p) olur. Ortadaki n değerleri eşleşmelidir.', code: 'Matrix left(2, 3);\nMatrix right(3, 4);\nMatrix result = left.multiply(right);\n// result: 2 × 4', note: 'Toplama aynı şekli, klasik çarpım ise iç boyut uyumunu ister.'
  },
  {
    id: '05', group: 'Matris temelleri', title: 'İndeksleme ve row-major bellek', slug: 'indeksleme', summary: 'İki boyutlu hücrenin tek boyutlu vector içindeki adresini hesapla.', why: 'MatrixFlash veriyi std::vector<double> içinde tutar. Bu, ardışık erişim ve cache davranışı üzerinde doğrudan etkilidir.', math: 'index = row × cols + col. 2×3 matriste a₁₁ hücresinin indeksi 1×3+1 = 4 olur.', code: 'std::vector<double> data;\n// data[row * cols + col]\ndouble value = data[1 * 3 + 1];', note: 'Görünen iki boyutlu model ile gerçek bellek düzenini birbirinden ayırmayı öğren.'
  },
  {
    id: '06', group: 'Temel API', title: 'Constructor ile matris oluştur', slug: 'constructor', summary: 'Matrix(rows, cols) çağrısının boyut ve başlangıç değerlerini nasıl kurduğunu öğren.', why: 'Her işlem önce geçerli bir Matrix nesnesine ihtiyaç duyar.', math: 'Constructor data.resize(rows×cols) ile gerekli hücre sayısını ayırır. Başlangıç değerleri double için sıfırdır.', code: 'Matrix weights(3, 2);\n// 3 satır, 2 sütun\n// başlangıç: [0 0; 0 0]', note: 'Negatif satır veya sütun sayısı invalid_argument üretir; hatayı sessizce yutmaz.'
  },
  {
    id: '07', group: 'Temel API', title: 'at ile güvenli hücre erişimi', slug: 'at-erisim', summary: 'Okuma, yazma, const erişim ve sınır kontrolünü gerçek kodla kullan.', why: 'at, öğrenme aşamasında geçersiz indeksleri erken yakalayan güvenli giriş kapısıdır.', math: '0 ≤ row < rows ve 0 ≤ col < cols koşulları sağlanmalıdır.', code: 'Matrix A(2, 2);\nA.at(0, 1) = 4.5;\nconst Matrix copy = A;\ndouble x = copy.at(0, 1);', note: 'Sıcak iç döngülerde at maliyeti istemiyorsan kütüphanenin mevcut ham pointer optimizasyonuna bak.'
  },
  {
    id: '08', group: 'Temel API', title: 'Kopya ve taşıma semantiği', slug: 'copy-move', summary: 'clone, copy constructor ve move assignment arasındaki farkı öğren.', why: 'Büyük matrislerde gereksiz kopya, matematikten çok bellek taşıma maliyeti yaratabilir.', math: 'Kopya bağımsız veri üretir. Move ise sahipliği aktarır ve kaynak nesneyi boş duruma getirir.', code: 'Matrix original(100, 100);\nMatrix copy = original;\nMatrix moved = std::move(copy);\nMatrix safe = original.clone();', note: 'Fonksiyonların Matrix döndürmesi move elision ve move constructor sayesinde verimli olabilir.'
  },
  {
    id: '09', group: 'Temel API', title: 'Factory fonksiyonları', slug: 'factory-matrisler', summary: 'zeros, ones, identity ve setIdentity ile kontrollü başlangıç matrisleri kur.', why: 'Sık kullanılan özel matrisleri elle hücre hücre doldurmak hem uzun hem de hataya açıktır.', math: 'Identity(n), köşegende 1, diğer hücrelerde 0 taşır ve A·I = A özelliğini sağlar.', code: 'Matrix zero = Matrix::zeros(2, 3);\nMatrix one = Matrix::ones(2, 3);\nMatrix eye = Matrix::identity(3);\neye.setIdentity();', note: 'identity yalnızca kare matris üretir; ones için sütun varsayılan olarak 1’dir.'
  },
  {
    id: '10', group: 'Aritmetik', title: 'Toplama ve çıkarma', slug: 'toplama-cikarma', summary: 'Eleman bazlı add ve subtract işlemlerini gradient mantığıyla bağla.', why: 'Ağırlık güncellemesi, bias ekleme ve hata hesapları bu iki işlemi sürekli kullanır.', math: 'Cᵢⱼ = Aᵢⱼ + Bᵢⱼ ve Dᵢⱼ = Aᵢⱼ − Bᵢⱼ. Şekiller birebir aynı olmalıdır.', code: 'Matrix updated = weights.subtract(\n    gradient.multiplyScalar(learningRate)\n);', note: 'Boyut uyuşmazlığı invalid_argument üretir; sessiz broadcasting burada yoktur.'
  },
  {
    id: '11', group: 'Aritmetik', title: 'Skaler çarpım', slug: 'skaler-carpim', summary: 'Bir öğrenme oranını veya katsayıyı bütün hücrelere uygulamayı öğren.', why: 'Gradient descent formülü W := W − η∇W, gradient matrisini bir skalerle çarpmayı gerektirir.', math: 'Cᵢⱼ = αAᵢⱼ. Matrisin şekli değişmez, yalnızca değerlerin ölçeği değişir.', code: 'const double learningRate = 0.01;\nMatrix step = gradient.multiplyScalar(learningRate);\nweights = weights.subtract(step);', note: 'multiplyScalar yeni matris döndürür; orijinal matrisi korur.'
  },
  {
    id: '12', group: 'Aritmetik', title: 'Hadamard ve klasik çarpım', slug: 'iki-carpim', summary: 'elementwiseMultiply ile multiply arasındaki kritik farkı gör.', why: 'İki çarpım aynı sembolle anlatılsa da farklı matematiksel anlamlara ve boyut kurallarına sahiptir.', math: 'Hadamard: Cᵢⱼ=AᵢⱼBᵢⱼ. Klasik çarpım: Cᵢⱼ=ΣₖAᵢₖBₖⱼ.', code: 'Matrix mask = activations.elementwiseMultiply(derivative);\nMatrix output = input.multiply(weights);', note: 'Backpropagation türev maskesi genellikle elementwise, katman geçişi ise klasik çarpımdır.'
  },
  {
    id: '13', group: 'Aritmetik', title: 'Matris çarpımının iç döngüsü', slug: 'matris-carpimi', summary: 'i-k-j döngü düzeninin formülü ve cache avantajını adım adım incele.', why: 'multiply, kütüphanenin en sıcak kod yollarından biridir; matematik kadar bellek erişimi de önemlidir.', math: 'Cᵢⱼ = Σₖ AᵢₖBₖⱼ. Her çıktı hücresi bir satır-sütun noktasal çarpımıdır.', code: 'for (int i = 0; i < m; ++i)\n  for (int k = 0; k < n; ++k)\n    for (int j = 0; j < p; ++j)\n      C[i * p + j] += A[i * n + k] * B[k * p + j];', note: 'i-k-j düzeninde B satırı ve C satırı ardışık okunur; bu otomatik vektörleşmeye yardım eder.'
  },
  {
    id: '14', group: 'Aritmetik', title: 'Broadcasting mantığı', slug: 'broadcasting', summary: '1×n satır vektörünü matrisin her satırına nasıl yayınlayacağını öğren.', why: 'Bias vektörü gibi küçük yapıları fiziksel olarak kopyalamadan büyük matrise uygulamak isteriz.', math: 'A(m×n) + b(1×n) sonucu her satır için Aᵢⱼ+b₀ⱼ hesaplar.', code: 'Matrix logits = input.multiply(weights);\nMatrix bias(1, logits.col_s());\nMatrix shifted = logits.addBroadcast(bias);', note: 'MatrixBroadcasting.cpp yalnızca uyumlu 1×n ve m×n şekillerini kabul eder.'
  },
  {
    id: '15', group: 'Dönüşümler', title: 'Transpose: satırı sütuna çevir', slug: 'transpose', summary: 'Devrik matrisin hem formülünü hem de backpropagation içindeki rolünü anla.', why: 'Ağırlık gradyanının boyutunu kurmak ve satır/sütun yönünü değiştirmek için temel araçtır.', math: 'Aᵀᵢⱼ = Aⱼᵢ. m×n matris transpose sonrası n×m olur.', code: 'Matrix inputT = input.transpose();\nMatrix weightGradient = inputT.multiply(error);', note: 'transpose yeni nesne döndürür; çağırdığın matris yerinde değişmez.'
  },
  {
    id: '16', group: 'Dönüşümler', title: 'Flatten ve veri şekillendirme', slug: 'flatten', summary: 'İki boyutlu veriyi tek satıra indirerek bir sonraki katmana hazırla.', why: 'Görüntü veya özellik matrisini dense katmana bağlarken tek vektör biçimine ihtiyaç duyarsın.', math: 'm×n → 1×(m·n). Veri sırası değişmez, yalnızca rows=1 ve cols=m·n yapılır.', code: 'Matrix features(2, 3);\nfeatures.flatten();\n// features: 1 × 6', note: 'Flatten kopya oluşturmaz; mevcut nesnenin boyut bilgisini yerinde günceller.'
  },
  {
    id: '17', group: 'Dönüşümler', title: 'Slice ile alt matris', slug: 'slice', summary: 'Bir matrisin satır ve sütun aralığından yeni bir görünüm üret.', why: 'Pencereleme, mini-batch seçimi ve özellik alanlarını ayırma için kullanışlıdır.', math: 'slice(rStart,rEnd,cStart,cEnd) başlangıcı dahil, bitişi hariç alır; Python aralıklarına benzer.', code: 'Matrix patch = image.slice(0, 3, 2, 5);\n// 3 satır × 3 sütun', note: 'Bitiş indeksini dahil etmeme kuralı, aralıkları birleştirirken sınır hatalarını azaltır.'
  },
  {
    id: '18', group: 'Dönüşümler', title: 'Concat ile birleştirme', slug: 'concat', summary: 'Matrisleri dikey, yatay veya genel concatenate API’siyle birleştir.', why: 'Batch’leri bir araya getirmek ve özellik matrislerini genişletmek için şekilleri kontrollü biçimde birleştirirsin.', math: 'Dikey concat: (m×n)+(p×n)=(m+p)×n. Yatay concat: (m×n)+(m×p)=m×(n+p).', code: 'Matrix batch = first.concatVertical(second);\nMatrix features = left.concatHorizontal(right);\nMatrix all = Matrix::concatenate({&first, &second}, 0);', note: 'axis=0 satır, axis=1 sütun boyunca birleştirme anlamına gelir.'
  },
  {
    id: '19', group: 'İstatistik', title: 'Mean ve ortalama', slug: 'mean', summary: 'Bir matrisin toplam seviyesini tek bir istatistikle özetle.', why: 'Normalizasyon öncesi veri kontrolü ve hızlı sanity check için basit ama değerli bir metriktir.', math: 'mean(A)= (1/N) Σᵢⱼ Aᵢⱼ. N toplam hücre sayısıdır.', code: 'double average = activations.mean();\nstd::cout << "mean: " << average << "\\n";', note: 'Mean satır bazlı değil, bütün matris üzerindeki genel ortalamadır.'
  },
  {
    id: '20', group: 'İstatistik', title: 'L1 ve L2 normları', slug: 'normlar', summary: 'Değer büyüklüğünü ve regularization sezgisini normlarla ölç.', why: 'Model ağırlıklarının ne kadar büyüdüğünü izlemek ve optimizasyon davranışını anlamak için kullanılır.', math: 'L1=Σ|xᵢ|. L2=√(Σxᵢ²). L1 seyrekliği, L2 ise enerjiyi ölçmeye yakındır.', code: 'double sparsity = weights.l1_norm();\ndouble energy = weights.l2_norm();', note: 'Normların ikisi de şekli değil, hücre değerlerinin toplam büyüklüğünü özetler.'
  },
  {
    id: '21', group: 'İstatistik', title: 'Trace ve negate', slug: 'trace-negate', summary: 'Köşegen toplamını ve işaret terslemeyi ileri işlemlere bağla.', why: 'Trace kare matrisin önemli bir özetidir; negate ise simetri ve test senaryolarında pratiktir.', math: 'tr(A)=ΣᵢAᵢᵢ. Negate(A)=-A her hücrede işaret değiştirir.', code: 'double diagonalSum = square.trace();\nweights.negate(); // yerinde -weights', note: 'Trace yalnızca kare matrislerde anlamlıdır ve kütüphane bunu kontrol eder.'
  },
  {
    id: '22', group: 'İleri cebir', title: 'Determinantın anlamı', slug: 'determinant', summary: 'Alan/hacim ölçeği, tekillik ve hızlı determinant yollarını öğren.', why: 'Determinant sıfırsa matris terslenemez; bu, çözüm yolları için kritik bir tanıdır.', math: '2×2 için det([[a,b],[c,d]])=ad−bc. n×n için LU sonrasında det(A)=işaret·ΠUᵢᵢ.', code: 'Matrix A(2, 2);\nA.at(0, 0) = 4; A.at(0, 1) = 7;\nA.at(1, 0) = 2; A.at(1, 1) = 6;\ndouble det = A.determinant(); // 10', note: 'Implementasyon 1×1, 2×2 ve 3×3 için özel hızlı yollar kullanır.'
  },
  {
    id: '23', group: 'İleri cebir', title: 'Inverse ile ters alma', slug: 'inverse', summary: 'Bir matrisin tersini LU ve ileri/geri yerine koyma üzerinden anla.', why: 'A⁻¹A=I ilişkisi, lineer sistemleri çözmenin ve dönüşümü geri almanın klasik yoludur.', math: 'A·A⁻¹=I. Ters yalnızca kare ve det(A)≠0 olan matrisler için vardır.', code: 'Matrix A(2, 2);\n// A değerlerini doldur\nMatrix inverse = A.Inverse();\nMatrix check = A.multiply(inverse);', note: 'Tekil matriste runtime_error gelir; sıfıra bölme sessizce devam etmez.'
  },
  {
    id: '24', group: 'İleri cebir', title: 'LU decomposition', slug: 'lu-ayristirma', summary: 'P·A=L·U sözleşmesini ve L, U, P matrislerinin rolünü çöz.', why: 'Tek bir ayrıştırmayı tekrar kullanmak, aynı matrisle birden çok sistemi çözmeyi hızlandırır.', math: 'P, satır takaslarını temsil eder. L alt üçgensel çarpanları, U üst üçgensel çarpanları taşır.', code: 'LUDecomposition parts = A.lu_decompose();\nMatrix left = parts.L;\nMatrix upper = parts.U;\nMatrix permutation = parts.P;', note: 'MatrixFlash pivoting sonrası P·A = L·U ilişkisini döndürür.'
  },
  {
    id: '25', group: 'İleri cebir', title: 'Pivoting ve sayısal kararlılık', slug: 'pivoting', summary: 'Partial pivoting’in neden gerekli olduğunu küçük bir matris üzerinden gör.', why: 'Küçük pivotlara bölmek yuvarlama hatasını büyütür; en büyük adayı seçmek kararlılığı artırır.', math: 'Her k adımında |Aᵢₖ| değeri en büyük olan satır pivot satırı seçilir ve k ile takas edilir.', code: 'Matrix pivoted(2, 2);\npivoted.at(0, 0) = 0;\npivoted.at(0, 1) = 2;\npivoted.at(1, 0) = 1;\npivoted.at(1, 1) = 3;\nauto parts = pivoted.lu_decompose();', note: 'P matrisi, yapılan satır takaslarının matematiksel izini korur.'
  },
  {
    id: '26', group: 'İleri cebir', title: 'Power iteration ve özdeğer', slug: 'power-iteration', summary: 'Baskın özdeğeri iteratif olarak yaklaşık hesapla.', why: 'PCA ve dinamik sistemler gibi alanlarda en baskın yönü bilmek değerli bir özet sağlar.', math: 'xₖ₊₁=A xₖ ardından normalize edilir. Yakınsayan yön baskın özvektör, oran baskın özdeğerdir.', code: 'auto [eigenvalue, eigenvector] =\n    matrix.power_iteration(1000, 1e-8);', note: 'Başlangıç vektörü ve tolerance, yakınsama davranışını etkiler; sonuç yaklaşık değerdir.'
  },
  {
    id: '27', group: 'Aktivasyonlar', title: 'Randomize ile ağırlık başlatma', slug: 'randomize', summary: 'Model ağırlıklarını küçük bir aralıkta rastgele üret ve sıfır simetrisini kır.', why: 'Bütün nöronlar aynı sıfır ağırlıkla başlarsa aynı gradyanı görür ve farklı uzmanlıklar geliştiremez.', math: 'wᵢ ~ Uniform(min,max). Küçük başlangıçlar aktivasyonları kontrol altında tutar.', code: 'Matrix weights(3, 2);\nweights.randomize(-0.1, 0.1);\nweights.randomize(-0.5, 0.5);', note: 'Aralık ters verilirse invalid_argument oluşur. Üretim modellerinde seed yönetimini ayrıca düşün.'
  },
  {
    id: '28', group: 'Aktivasyonlar', title: 'ReLU aktivasyonu', slug: 'relu', summary: 'Negatif sinyalleri sıfırlayıp pozitif sinyalleri koruyan aktivasyonu uygula.', why: 'ReLU, lineer katmanların tek başına ifade edemediği doğrusal olmayan ilişkileri öğrenmeye yardım eder.', math: 'ReLU(x)=max(0,x). Türev pozitif bölgede 1, negatif bölgede 0 kabul edilir.', code: 'Matrix scores = input.multiply(weights);\nscores.addBroadcast(bias);\nscores.relu();', note: 'relu yerinde çalışır; kopya korumak istiyorsan clone ile önce kopyala.'
  },
  {
    id: '29', group: 'Aktivasyonlar', title: 'Softmax ve sayısal kararlılık', slug: 'softmax', summary: 'Logit değerlerini satır bazlı olasılıklara dönüştür ve overflow’dan kaçın.', why: 'Sınıflandırmada model skorlarını toplamı 1 olan yorumlanabilir değerlere çevirir.', math: 'softmax(xᵢ)=eˣⁱ/Σⱼeˣʲ. Kararlı biçim e^(xᵢ−max(x)) kullanır.', code: 'Matrix logits(1, 3);\nlogits.at(0, 0) = 2.0;\nlogits.at(0, 1) = 1.0;\nlogits.softmax();', note: 'softmax her satırı ayrı normalize eder; NewSoftMax orijinali değiştirmeden sonuç üretir.'
  },
  {
    id: '30', group: 'Aktivasyonlar', title: 'Aktivasyon pipeline’ı', slug: 'aktivasyon-pipeline', summary: 'Linear → bias → ReLU → Softmax akışını tek bir model parçasında kur.', why: 'Gerçek bir yapay zeka katmanı, tek bir matematik işlemi değil, şekli korunan bir işlem zinciridir.', math: 'y=softmax(ReLU(xW+b)). Her adımın giriş/çıkış boyutunu takip etmelisin.', code: 'Matrix hidden = input.multiply(weights);\nhidden = hidden.addBroadcast(bias);\nhidden.relu();\nMatrix probabilities = hidden.NewSoftMax();', note: 'İşlemleri isimli ara matrislerle yazmak, boyut ve hata ayıklamayı kolaylaştırır.'
  },
  {
    id: '31', group: 'Dosya ve güvenlik', title: 'Binary serialization', slug: 'serialization', summary: 'Matris boyutlarını ve ham double verisini dosyada nasıl saklayacağını öğren.', why: 'Model ağırlıklarını tekrar kullanmak için hızlı ve basit bir kalıcı format gerekir.', math: 'Dosya düzeni: int rows + int cols + rows×cols adet double. Okuma sırası yazma sırasıyla aynı olmalıdır.', code: 'weights.saveBinary("weights.bin");\nMatrix restored = Matrix::loadBinary("weights.bin");', note: 'loadBinary eksik başlık, negatif boyut veya kesilmiş veri gördüğünde runtime_error üretir.'
  },
  {
    id: '32', group: 'Dosya ve güvenlik', title: 'Hata yönetimi sözleşmesi', slug: 'hata-yonetimi', summary: 'invalid_argument, out_of_range ve runtime_error ayrımını gerçek çağrılarla öğren.', why: 'Hatanın türünü bilmek, kullanıcı hatasını program arızasından ayırmayı sağlar.', math: 'Geçersiz şekil bir önkoşul ihlalidir; tekil matris ise işlem sırasında keşfedilen sayısal durumdur.', code: 'try {\n    Matrix result = left.multiply(right);\n} catch (const std::invalid_argument& error) {\n    std::cerr << error.what();\n}', note: 'Kütüphane hatayı yutmaz ve başarısız sonucu sessizce döndürmez.'
  },
  {
    id: '33', group: 'Build sistemi', title: 'CMake ile MatrixLib', slug: 'cmake', summary: 'Kütüphaneyi hedefe bağlama, include dizinini verme ve örnekleri derleme.', why: 'Birden çok cpp dosyasının doğru linklenmesi için tek dosya çalıştırıcısı yerine CMake hedefi gerekir.', math: 'Derleme (compile) her kaynak dosyasını üretir; link aşaması bunları MatrixLib statik kütüphanesinde birleştirir.', code: 'add_library(MatrixLib STATIC\n  src/Matrix.cpp\n  src/MatrixArithmetic.cpp\n  src/MatrixActivation.cpp\n)\ntarget_include_directories(MatrixLib PUBLIC include)', note: 'Uygulama hedefi target_link_libraries(MatrixApp PRIVATE MatrixLib) ile bağlanır.'
  },
  {
    id: '34', group: 'Build sistemi', title: 'Testleri ve CTest’i çalıştır', slug: 'testler', summary: 'Bir matematik kütüphanesinde davranış testinin neden vazgeçilmez olduğunu gör.', why: 'Bir algoritmayı düzeltirken determinant, pivot, boyut ve serialization gibi kenar durumları korumak gerekir.', math: 'Test yalnızca beklenen sayıyı değil, değişmezliği de kontrol eder: A·A⁻¹ ≈ I gibi.', code: 'cmake --build build --target MatrixTests --parallel\nctest --test-dir build --output-on-failure', note: 'Floating point karşılaştırmalarında mutlak epsilon kullan; double değerleri doğrudan == ile kıyaslama.'
  },
  {
    id: '35', group: 'Yapay zeka', title: 'Layer fikrini matrise çevir', slug: 'layer', summary: 'Bir dense katmanın input, weights, bias ve output ilişkisini tasarla.', why: 'MatrixFlash’ın işlemleri tek başına değil, katman gibi daha büyük soyutlamaların içinde değer kazanır.', math: 'Dense katman: z=xW+b. x 1×n, W n×m, b 1×m ve z 1×m olur.', code: 'struct Layer {\n    Matrix weights;\n    Matrix bias;\n\n    Matrix forward(const Matrix& input) const {\n        return input.multiply(weights).addBroadcast(bias);\n    }\n};', note: 'Katman, matrisin içine girmez; matris operasyonlarını anlamlı bir akışta orkestre eder.'
  },
  {
    id: '36', group: 'Yapay zeka', title: 'Forward pass adımları', slug: 'forward-pass', summary: 'Bir girdinin ağırlıklar boyunca ilerleyip tahmine dönüşmesini takip et.', why: 'Forward pass, modelin mevcut parametrelerle ne tahmin ettiğini hesaplayan saf akıştır.', math: 'a⁰=x, z¹=a⁰W¹+b¹, a¹=ReLU(z¹), ŷ=softmax(a¹W²+b²).', code: 'Matrix hidden = input.multiply(layer1.weights);\nhidden = hidden.addBroadcast(layer1.bias);\nhidden.relu();\nMatrix logits = hidden.multiply(layer2.weights);\nlogits = logits.addBroadcast(layer2.bias);', note: 'Her ara değeri isimlendirerek hangi katmanda şeklin bozulduğunu kolayca görebilirsin.'
  },
  {
    id: '37', group: 'Yapay zeka', title: 'Gradient descent', slug: 'gradient-descent', summary: 'Kayıp eğimini kullanarak ağırlıkların nasıl güncellendiğini uygula.', why: 'Öğrenme, modelin tahmin hatasını azaltacak parametre yönünde küçük adımlarla ilerlemesidir.', math: 'Wₜ₊₁=Wₜ−η∇W. η learning rate, ∇W kaybın ağırlığa göre türevidir.', code: 'Matrix step = gradient.multiplyScalar(learningRate);\nweights = weights.subtract(step);', note: 'Learning rate çok büyükse kayıp sıçrar; çok küçükse öğrenme yavaşlar.'
  },
  {
    id: '38', group: 'Yapay zeka', title: 'Backpropagation sezgisi', slug: 'backpropagation', summary: 'Hatanın katmanlar boyunca transpose ve Hadamard çarpımıyla geri akışını anla.', why: 'Forward tahmini üretir; backpropagation hangi parametrenin ne kadar sorumlu olduğunu bulur.', math: 'δˡ=(δˡ⁺¹Wᵀ)⊙f′(zˡ). Burada transpose yönü, elementwise çarpım ise aktivasyon türevini taşır.', code: 'Matrix propagated = error.multiply(weights.transpose());\nMatrix localError = propagated.elementwiseMultiply(reluMask);\nMatrix gradient = input.transpose().multiply(localError);', note: 'Bu sayfa formülü ezberletmekten çok her matrisin shape’ini izlemeyi hedefler.'
  },
  {
    id: '39', group: 'Performans', title: 'CPU ve cache düşüncesi', slug: 'performans', summary: 'Row-major düzen, ardışık erişim, pointer kullanımı ve optimizasyon bayraklarını bağla.', why: 'Doğru algoritma kadar CPU’nun veriyi cache satırlarına nasıl taşıdığı da süreyi etkiler.', math: 'Aynı O(mnp) karmaşıklığındaki iki döngü düzeni, farklı bellek erişimi yüzünden farklı gerçek süreler verebilir.', code: 'for (int i = 0; i < rows; ++i) {\n    double* row = data.data() + i * cols;\n    for (int j = 0; j < cols; ++j) {\n        row[j] *= scalar;\n    }\n}', note: 'Erken optimizasyondan önce profil çıkar; ama sıcak döngülerde gereksiz sınır kontrolünü de fark et.'
  },
  {
    id: '40', group: 'Bitirme projesi', title: 'Kendi mini modelini kur', slug: 'bitirme-projesi', summary: 'Tüm öğrendiklerini bir veri akışında birleştir ve MatrixFlash üzerinde küçük bir model yaz.', why: 'Kavrayış, API çağrılarını ezberlemekten değil; boyut, matematik ve kod akışını birlikte kurabilmekten gelir.', math: 'input → linear → bias → ReLU → linear → Softmax → loss → gradient → update döngüsünü kur.', code: 'Matrix x(1, 3);\nx.randomize(-1.0, 1.0);\nMatrix hidden = x.multiply(w1).addBroadcast(b1);\nhidden.relu();\nMatrix prediction = hidden.multiply(w2).addBroadcast(b2);\nprediction.softmax();', note: 'Bir sonraki adım: tests/MatrixTests.cpp içine kendi edge-case testini ekle ve CMake ile doğrula.'
  }
];
