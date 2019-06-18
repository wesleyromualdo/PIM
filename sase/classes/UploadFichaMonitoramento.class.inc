<?php

/**
 * 
 * User: olaviojunior
 * Date: 28/03/2018
 * Time: 13:50
 */
class UploadFichaMonitoramento extends Modelo
{
    //Mensagem de erro ao verificarErroUpload
    private $erroUpload = false;

    //Array com as extensoes permitidas
    protected $allowed = array('xlsx', 'xls');

    //A extensão do arquivo enviado
    private $extension;

    //O nome permitido das planilhas do arquivo recebido
    private $arrSheetAllowed;

    //Os titulos das colunas presente na linha 1 da planilha
    private $arrTitulos;
    public $arquivo;

    public function __construct()
    {
        parent::__construct();
        include_once APPRAIZ . 'includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel.php';
        $this->arquivo = new FilesSimec();
    }

    /**
     * @return bool|string
     */
    public function getErroUpload()
    {
        return $this->erroUpload;
    }

    /**
     * @param bool $erroUpload
     */
    public function setErroUpload($erroUpload)
    {
        $this->erroUpload = $erroUpload;
    }

    /**
     * @return mixed
     */
    public function getExtension()
    {
        return $this->extension;
    }

    /**
     * @param mixed $extension
     */
    public function setExtension($extension)
    {
        $this->extension = $extension;
    }

    /**
     * @return mixed
     */
    public function getArrSheetAllowed()
    {
        return $this->arrSheetAllowed;
    }

    /**
     * @param mixed $arrSheetAllowed
     */
    public function setArrSheetAllowed($arrSheetAllowed)
    {
        $arrSheetAllowed = array_flip($arrSheetAllowed);
        foreach ($arrSheetAllowed as $index => $item) {
            $arrSheetAllowed[$index] = false;
        }
        $this->arrSheetAllowed = $arrSheetAllowed;
        //$arrSheetAllowed = array('metas' => false, 'submetas' => false, 'estrategias' => false);
    }

    /**
     * @return array
     */
    public function getArrTitulos()
    {
        return $this->arrTitulos;
    }

    /**
     * @param array $arrTitulos
     */
    public function setArrTitulos($arrTitulos)
    {
        $this->arrTitulos = $arrTitulos;
    }

    public function verificarErroUpload()
    {
        try {
            if (!isset($_FILES['userfile']['error']) || is_array($_FILES['userfile']['error'])) {
                throw new RuntimeException("<div class='alert alert-danger row col-sm-7' role='alert'><strong>Parâmetros inválidos.</strong></div><br>");
            }

            switch ($_FILES['userfile']['error']) {
                case UPLOAD_ERR_OK:
                    break;
                case UPLOAD_ERR_NO_FILE:
                    throw new RuntimeException("<div class='alert alert-danger row col-sm-7' role='alert'><strong>Nenhum arquivo enviado.</strong></div><br>");
                case UPLOAD_ERR_INI_SIZE:
                case UPLOAD_ERR_FORM_SIZE:
                    throw new RuntimeException("<div class='alert alert-danger row col-sm-7' role='alert'><strong>O tamanho do anexo excede o limite permitido (4MB).</strong></div><br>");
                default:
                    throw new RuntimeException("<div class='alert alert-danger row col-sm-7' role='alert'><strong>Erros desconhecidos.</strong></div><br>");
            }

//            $ext = pathinfo($_FILES['userfile']['name'], PATHINFO_EXTENSION);
            $this->setExtension(pathinfo($_FILES['userfile']['name'], PATHINFO_EXTENSION));

            if (!in_array($this->getExtension(), $this->allowed)) {
                throw new RuntimeException("<div class='alert alert-danger row col-sm-7' role='alert'><strong>A extensão do arquivo é inválida.</strong></div><br>");
            }

            return $this;

        } catch (RuntimeException $e) {
            setErroUpload($e->getMessage());

            return $this;
        }
    }
    public function retornaAnoPrazo($str)
    {
        $str = preg_replace("/[^0-9]/", "", $str);
        $str = (strlen($str) > 4) ? substr($str, -4) : $str;
        return $str;
    }
    public function mantemValor($str)
    {
        //return preg_replace("/[^0-9,]/", "", $str);
        return preg_replace("/[^0-9,.]/", "", $str);
        //return $str;
    }    
    public function removeTodosEspacos($str)
    {
        return preg_replace('/\s+/', '', trim(iconv("UTF-8","ISO-8859-1",$str)," \t\n\r\0\x0B\xA0"));
    }
    function IsNullOrEmptyString($str){
        return (!isset($str) || trim($str)==='');
    }
    function IsDecimal($str){
        //return strpos($str, ',');
        if (strpos($str, '.') !== false) {
            return true;
        } else {
            return false;
        }
    }    
    
    public function carregarDadosPlanilha()
    {        
        //INICIAR VARIÁVEIS DO PLUGIN PHPEXCEL

        switch ($this->getExtension()) {
            case 'xls':
                $objReader = PHPExcel_IOFactory::createReader('Excel5');
                break;
            case 'xlsx':
                $objReader = PHPExcel_IOFactory::createReader('Excel2007');
                break;
        }

        $objPHPExcel = $objReader->load($_FILES['userfile']['tmp_name']);

        $arrTotal = array();
        $sheets = $objPHPExcel->getSheetNames();
        $arrSheetAllowed = $this->getArrSheetAllowed();

        $qtdPlanilhasValidas = 0;
        //inicialiaza os indices das planilhas permitidas
        foreach ($sheets as $key => $sheet) {
            $sheet = strtolower(trim(removeAcentos(($sheet))));
            $sheet = $sheet === 'submetas' ? 'indicadores' : $sheet;
            if (key_exists($sheet, $arrSheetAllowed)){
                $arrSheetAllowed[$sheet] = $key;
                $qtdPlanilhasValidas++;
            } 
        }

        if ($qtdPlanilhasValidas > 0){
            $max = 1;
            foreach ($arrSheetAllowed as $nome => $index) {
    
                if ($index !== false) {
    
                    $indexArrTotal = 0;
                    $max1 = $objPHPExcel->setActiveSheetIndex($index)->getHighestDataRow();
                    $max2 = $objPHPExcel->setActiveSheetIndex($index)->getHighestDataColumn();
                    $colNumber = PHPExcel_Cell::columnIndexFromString($max2);
    
                    if ($max1 == 1) { // planilha tem titulo e não valores
                        $arrTotal[$nome] = false;
                    } else {
                        //LER A PLANILHA E INSERIR OS VALORES NO ARRAY $arrTotal
                        $objPHPExcel->setActiveSheetIndex($index);
    
                        //pega titulos
                        for ($j = 0; $j < $colNumber; $j++) {
                            $titulos[] = ($objPHPExcel->getActiveSheet()->getCellByColumnAndRow($j, 1)->getValue());
                        }
    
                        $this->setArrTitulos($titulos);
    
                        for ($i = '2'; $i <= $max1; $i++) {
    
                            for ($j = 0; $j < $colNumber; $j++) {
    
                                if ($sheets[0] == 'Monitoramento de Metas'
                                    && ($j == 3 || $j == 4 || $j == 5)) {
                                    $dado = $objPHPExcel->getActiveSheet()->getCellByColumnAndRow($j, $i)->getFormattedValue();
                                    $dado = str_replace(',', '.', $dado);
    
                                    $arrTotal[$nome][$i][$j] = ($dado);
    
                                } else {
                                    $dado = $objPHPExcel->getActiveSheet()->getCellByColumnAndRow($j, $i)->getFormattedValue();
                                    $arrTotal[$nome][$i][$j] = ($dado);
                                }
    
                            }
                            $indexArrTotal++;
                        }
                    }
                } else {
                    $arrTotal[$nome] = false;
                }
            } // fim foreach $arrSheetAllowed 
        } // fim if ($qtdPlanilhasValidas > 0)

        return $arrTotal;

    }

    /**
     * METODO PARA FAZER O UPLOAD DE INFORMAÇÕES DA FICHA B
     * @return bool
     */
    public function uploadFichaB()
    {
        //VERIFICAR ERROS
        if ($this->getErroUpload() !== false) {
            $return = json_encode(array('resultado' => 'erro', 'msg' => $this->getErroUpload()));

            return $return;
        }

        /**
         * O nome permitido das planilhas do arquivo recebido
         */

        $this->setArrSheetAllowed(array('metas', 'submetas', 'indicadores', 'estrategias'));
        $arrTotal = $this->carregarDadosPlanilha();
        
        //PREPARA PARA INSERIR

        $indexErros = 0;

        /**
         * Dados Planilha Metas
         */
        if (is_array($arrTotal['metas'])) {

            $selectInsertMetas = 'WITH inserts (muncod, estuf, fmaid, fbmnumero
            , fbmdescricao, fbmstatus, linha ) AS (VALUES ';

            $insertMetas = 'INSERT INTO sase.fichabmeta (
                muncod
                ,estuf
                ,fmaid
                ,usucpf
                ,fbmnumero
                ,fbmdescricao
                ,fbmprazo
                ,fbmalcancou
                ) VALUES ';

            $sqlHistoricoMeta = "INSERT INTO sase.fichabmeta_historico 
                                            (huscpf, husacao, husdtcriacao, usucpf, fbmnumero,fbmalcancou,fbmprazo,fbmdescricao,fmaid,estuf,muncod,fbmid)
                                     VALUES ";

            foreach ($arrTotal['metas'] as $key => $meta) {

                /**
                 * Retira as linha em branco
                 */
                $countColunas = strlen(trim($meta[0])) + strlen(trim($meta[1]))
                    + strlen(trim($meta[2])) + strlen(trim($meta[3]));
                if ($countColunas == 0) {
                    unset($arrTotal['metas'][$key]);
                    continue;
                }
                /*
                    Remover espaços em branco
                    Substituir virgula por ponto
                    Remover pontos consecutivos
                */
                $meta[0] = $this->removeTodosEspacos($meta[0]);                
                $meta[0] = str_replace(',', '.', $meta[0]);
                $meta[0] = str_replace('..', '.', $meta[0]);
                $meta[0] = str_replace('..', '.', $meta[0]);                

                /**
                 * O campo Número da meta deve permitir até 10 caracteres.
                 */
                if (strlen($meta[0]) > 10) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> A. A quantidade de caracteres foi excedida. Permitido: 10 caracteres.";
                    $indexErros++;
                }

                /**
                 * O campo Número da meta Não pode conter espaços
                 * e deve ser do tipo numérico.
                 */

                if (substr_count($meta[0], ' ') > 0) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> A. Não é permitido espaço nesta coluna.";
                    $indexErros++;
                } elseif (!is_numeric($meta[0])) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> A. Valor inválido. Permitido: Somente números.";
                    $indexErros++;
                }

                /**
                 * O campo Número da meta não pode estar repetido
                 */
                $arrAux = $arrTotal['metas'];
                unset($arrAux[$key]);
                if (in_array($meta[0], array_column($arrAux, 0)) && strlen(trim($meta[0])) != 0) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> A. Nº da Meta aparece duas vezes na planilha. O Nº da Meta não deve ser informado mais de uma vez.";

                    $indexErros++;
                }

                /**
                 * O campo Descrição da Meta não pode estar vazio.
                 */
                if (strlen($meta[1]) == 0) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> B. A descrição da Meta não pode estar vazia.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Meta deve permitir até 5000 caracteres.
                 */
                if (strlen($meta[1]) > 5000) {
                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> B. A quantidade de caracteres foi excedida. Permitido: 5000 caraceteres.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Meta não pode estar repetido
                 */
                //if (in_array($meta[1], array_column($arrAux, 1)) && strlen(trim($meta[1])) != 0) {
                //    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> B. Esta Descrição da Meta aparece duas vezes na planilha. A descrição da Meta não deve ser informada mais de uma vez.";
                //    $indexErros++;
                //}
                /**
                 * O campo "Prazo" deve ser igual ou maior que 2014 e igual ou menor que 2030
                 */
//                if ($meta[2] < 2014 || $meta[2] > 2030) {
//                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> C. O ano informado no campo \"Prazo\" deve ser igual ou maior que 2014 e igual ou menor que 2030 ";
//                    $indexErros++;
//                }

                /**
                 * O campo "Alcançou Meta?" somente pode ter os valores "Sim" e "Não"
                 */
//                $campoAlcancou = strtolower(trim(removeAcentos($meta[3])));
//                if ($campoAlcancou != 'nao' && $campoAlcancou != 'sim') {
//                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha</b> {$key}; <b>Coluna</b> D Valor inválido no campo \"Alcançou Meta?\". Permitido: Sim ou Não.";
//                    $indexErros++;
//                }

                /**
                 * Concatenando Insert
                 */
                $arrTotal['metas'][$key]['insert'] = "(" . $_POST['muncod'] . ",'" . $_POST['estuf'] . "'
                    ," . $_POST['fmaid'] . ",'" . $_POST['usucpf'] . "'," . $meta[0] . ",'" . trim($meta[1]) . "',
                    NULL ,NULL), ";

                $insertMetas .= $arrTotal['metas'][$key]['insert'];
                /**
                 * Concatenando select for insert
                 */
                $arrTotal['metas'][$key]['selectInsert'] = "(" . $_POST['muncod'] . ",'" . $_POST['estuf'] . "'
                    ," . $_POST['fmaid'] . "," . $meta[0] . ",'" . trim($meta[1]) . "',
                    'A','" . $key . "'), ";

                $selectInsertMetas .= $arrTotal['metas'][$key]['selectInsert'];

                /**
                 * Concatenando Historico Metas
                 */

                $arrTotal['metas'][$key]['insertHistorico'] = "
                ('{$_SESSION['usucpf']}','CREATE','" . date('Y-m-d H:m:s') . "','{$_SESSION['usucpf']}'
                ,{$meta[0]},NULL,NULL,'" . trim($meta[1]) . "',{$_POST['fmaid']},'{$_POST['estuf']}',{$_POST['muncod']},HFBMID ),";

            } //fim foreach

            $insertMetas = substr($insertMetas, 0, -2) . ' RETURNING fbmid,fbmnumero ';

            $estuf = $muncod = '';

            if ($_REQUEST['muncod'] != 'null'){
                $muncod = " AND meta.muncod = '{$_REQUEST['muncod']}' ";
            }

            if ($_REQUEST['muncod'] == 'null' && $_REQUEST['estuf'] != 'null'){
                $estuf = " AND meta.estuf = '{$_REQUEST['estuf']}' ";
            }

            $selectInsertMetas = substr($selectInsertMetas, 0, -2);
            $selectInsertMetas .= ' )
                SELECT meta.fbmid, meta.fbmnumero, meta.fbmdescricao, inserts.linha
                FROM sase.fichabmeta meta
                  JOIN inserts ON
                        meta.estuf = inserts.estuf
                        AND meta.fbmdescricao = inserts.fbmdescricao
                        AND meta.fbmnumero = inserts.fbmnumero
                        AND meta.fbmstatus = inserts.fbmstatus'
                        .$muncod. '' .$estuf;

        } // fim if (is_array($arrTotal['metas']))
        else 
        {
            // não existe a aba Metas!
            $erro[$indexErros] = "<b>Aba</b> Metas: Este arquivo não possui uma aba Metas!";
            $indexErros++;
        }
        /**
         * Dados Planilha indicadores
         */
        if (is_array($arrTotal['indicadores'])) {

            $selectInsertSubMetas = 'WITH inserts ( fbsnumero, fbsdescricao
                , fbsstatus, linha ) AS (VALUES  ';

            foreach ($arrTotal['indicadores'] as $key => $submeta) {                
                /**
                 * Retira as linha em branco
                 */
                $countColunas = strlen(trim($submeta[0])) + strlen(trim($submeta[1]))
                    + strlen(trim($submeta[2])) + strlen(trim($submeta[3]));
                if ($countColunas == 0) {
                    unset($arrTotal['indicadores'][$key]);
                    continue;
                }

//                $submeta[0] = trim(iconv("UTF-8","ISO-8859-1",$submeta[0])," \t\n\r\0\x0B\xA0");
//                $submeta[0] = preg_replace('/\s+/', '', $submeta[0]);
                /*
                    Remover espaços em branco
                    Substituir virgula por ponto
                    Remover pontos consecutivos
                */
                $submeta[0] = $this->removeTodosEspacos($submeta[0]);                
                $submeta[0] = str_replace(',', '.', $submeta[0]);
                $submeta[0] = str_replace('..', '.', $submeta[0]);
                $submeta[0] = str_replace('..', '.', $submeta[0]);
                // Limpar a coluna 'Prazo'
                //$submeta[2] = preg_replace("/[^0-9]/", "", $submeta[2]);
                $submeta[2] = $this->retornaAnoPrazo($submeta[2]);
                

                /**
                 * O campo Número da Submeta deve permitir até 10 caracteres.
                 */
                if (strlen($submeta[0]) > 10) {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> A. *{$submeta[0]}* A quantidade de caracteres foi excedida. Permitido: 10 caracteres.";
                    $indexErros++;
                }

                /**
                 * O código da submeta será validado pelas casas decimais antes do ponto.
                 * Exemplo: se uma meta possuir número igual a "20" e uma submeta foi
                 * cadastrada com número igual a "20.A", a submeta deve ser vinculada a meta.
                 *
                 * Se uma submeta possuir número que não condiz com nenhuma meta cadastrada,
                 * o sistema não deve realizar a importação e o erro deve ser apresentado no
                 * relatório de erro.
                 */
                $arrAux = $arrTotal['metas'];
                $arrTotal['indicadores'][$key]['numeroMeta'] = substr($submeta[0], 0, strpos($submeta[0], '.'));

                if (!in_array($arrTotal['indicadores'][$key]['numeroMeta'], array_column($arrAux, 0))) {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> A. Esta Submeta *({$submeta[0]})* não está vinculada a uma meta na aba Metas. Permitido: Nº da Meta.Nº do Indicador.";
                    $indexErros++;
                }

                /**
                 * O campo Número da Submeta não pode estar repetido
                 */
                $arrAux = $arrTotal['indicadores'];
                unset($arrAux[$key]);

                $array_column = array_column($arrAux, 0);

                if (in_array($submeta[0], $array_column) && strlen(trim($submeta[0])) != 0) {
                    $array_keys = array_keys($array_column, $submeta[0]);

                    foreach ($array_keys as $key2) {
                        if ($submeta[0] === $array_column[$key2]) {
                            $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> A. Nº do Indicador aparece duas vezes na planilha. O Nº do Indicador não deve ser informado mais de uma vez.";
                            $indexErros++;
                        }
                    }
                }

                // Limpar a coluna 'Descrição do Indicador'
                //$submeta[1] = trim(iconv("UTF-8","ISO-8859-1",$submeta[1])," \t\n\r\0\x0B\xA0");                
                
                /**
                 * O campo Descrição da Submeta não pode estar vazio.
                 */
                if (strlen($submeta[1]) == 0) {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> B. A coluna 'Descrição do Indicador' não pode estar vazia.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Submeta deve permitir até 500 caracteres.
                 */
                if (strlen($submeta[1]) > 5000) {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> B. A quantidade de caracteres foi excedida. Permitido: 5000 caraceteres.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Submeta não pode estar repetido
                 */
                //if (in_array($submeta[1], array_column($arrAux, 1))) {
                //    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> B. Esta Descrição da Submeta aparece duas vezes na planilha. A descrição da Submeta não deve ser informada mais de uma vez.";
                //    $indexErros++;
                //}
                
                /**
                 * O campo "Prazo" deve ser igual ou maior que 2014 e igual ou menor que 2030
                 */
                if ($submeta[2] < 2014 || $submeta[2] > 2030) {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> C. O ano informado *({$submeta[2]})* no campo \"Prazo\" deve ser igual ou maior que 2014 e igual ou menor que 2030.";
                    $indexErros++;
                }

                /**
                 * O campo "Alcançou SubMeta?" somente pode ter os valores "Sim" e "Não"
                 */
                $campoAlcancou = strtolower(trim(removeAcentos($submeta[3])));
                if ($campoAlcancou != 'nao' && $campoAlcancou != 'sim') {
                    $erro[$indexErros] = "<b>Aba</b> Indicadores; <b>Linha</b> {$key}; <b>Coluna</b> D. Valor inválido no campo \"Alcançou Meta?\". Permitido: Sim ou Não.";
                    $indexErros++;
                }

                $numeroSubmeta = substr($submeta[0], strpos($submeta[0], '.') + 1);

                /**
                 * Concatenando Insert
                 */
                $arrTotal['indicadores'][$key]['insert'] =
                    "(" . 'FBMID' . ",'" . $_POST['usucpf'] . "', '" . $numeroSubmeta . "'
                    ,'" . trim($submeta[1]) . "', '" . $submeta[2] . "',
                    " . ($campoAlcancou == 'nao' ? 'false' : 'true') . "), ";

                /**
                 * Concatenando select for insert
                 */

                $arrTotal['indicadores'][$key]['selectInsert'] =
                    "( '" . $numeroSubmeta . "'
                    ,'" . trim($submeta[1]) . "', 'A','" . $key . "' ), ";

                $selectInsertSubMetas .= $arrTotal['indicadores'][$key]['selectInsert'];

                /**
                 * Concatenando Historico SubMetas
                 */

                $arrTotal['indicadores'][$key]['insertHistorico'] = "
                ('{$_SESSION['usucpf']}','CREATE','" . date('Y-m-d H:m:s') . "','{$_SESSION['usucpf']}'
                ,'{$numeroSubmeta}'," . ($campoAlcancou == 'nao' ? 'false' : 'true') . ",{$submeta[2]},'" . trim($submeta[1]) . "',HFBSID,HFBMID ),";


            } //fim foreach

            $selectInsertSubMetas = substr($selectInsertSubMetas, 0, -2);
            $selectInsertSubMetas .= ' ) SELECT  distinct submeta.fbsnumero ,submeta.fbsdescricao, inserts.linha
				FROM sase.fichabsubmeta submeta
				JOIN inserts ON submeta.fbsdescricao = inserts.fbsdescricao
					AND submeta.fbsnumero = inserts.fbsnumero
					AND submeta.fbsstatus = inserts.fbsstatus';

        } //fim if (is_array($arrTotal['indicadores']))
        else 
        {
            // não existe a aba Indicadores!
            $erro[$indexErros] = "<b>Aba</b> Indicadores: Este arquivo não possui uma aba Indicadores!";
            $indexErros++;
        }
        /**
         * Dados planilha Estrategias
         */
        if (is_array($arrTotal['estrategias'])) {

            $selectInsertEstrategias = 'WITH inserts ( fbenumero, fbedescricao
                , fbestatus, linha ) AS ( VALUES ';

            foreach ($arrTotal['estrategias'] as $key => $estrategia) {
                /**
                 * Retira as linha em branco
                 */
                $countColunas = strlen(trim($estrategia[0])) + strlen(trim($estrategia[1]))
                    + strlen(trim($estrategia[2])) + strlen(trim($estrategia[3]));
                if ($countColunas == 0) {
                    unset($arrTotal['estrategias'][$key]);
                    continue;
                }

                /*
                    Limpar a coluna 'Número da Estratégia' 
                    Remover espaços em branco
                    Substituir virgula por ponto
                    Remover pontos consecutivos
                */
                $estrategia[0] = $this->removeTodosEspacos($estrategia[0]);// preg_replace('/\s+/', '', trim(iconv("UTF-8","ISO-8859-1",$estrategia[0])," \t\n\r\0\x0B\xA0"));
                $estrategia[0] = str_replace(',', '.', $estrategia[0]);
                $estrategia[0] = str_replace('..', '.', $estrategia[0]);
                $estrategia[0] = str_replace('..', '.', $estrategia[0]);
                // Limpar a coluna 'Prazo'
               // $estrategia[2] = ereg_replace("/[^0-9]/", "", $estrategia[2]);
               $estrategia[2] = $this->retornaAnoPrazo($estrategia[2]); //preg_replace("/[^0-9]/", "", $estrategia[2]);
                
                /**
                 * O campo Número da Estratégia deve permitir até 10 caracteres.
                 */

                if (strlen($estrategia[0]) > 10) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. A quantidade de caracteres foi excedida. Permitido: 10 caracteres.";
                    $indexErros++;
                }

                /**
                 * O campo Número da Estratégia deve seguir o padrao nºmeta.nºestrategia.
                 */
                $numeroEstrategia = substr($estrategia[0], strpos($estrategia[0], '.') + 1);

                if ($numeroEstrategia == false) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. O campo Número da Estratégia deve seguir o padrao [Nºmeta].[Nºestrategia].";
                    $indexErros++;
                }

                /**
                 * O campo Número da Estratégia Não pode conter espaços;
                 *
                 * O sistema não deve permitir pontos consecutivos (ex 1...) na coluna número da estratégia.
                 * A seguinte mensagem deve ser apresentada ao usuário no relatório de erro:
                 * ?Não é permitido pontos consecutivos nesta coluna?;
                 *
                 * O campo Número da Estratégia deve ser do tipo numérico.
                 */

                if (substr_count($estrategia[0], ' ') > 0) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. Não é permitido espaço nesta coluna.";
                    $indexErros++;
                } elseif (strpos($estrategia[0], '..') !== false) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. Não é permitido pontos consecutivos nesta coluna.";
                    $indexErros++;

                } else {
                    $auxEstrategia = explode('.', $estrategia[0]);
                    foreach ($auxEstrategia as $value) {
                        if (!is_numeric($value)) {
                            $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. Valor inválido. Permitido: Somente números.";
                            $indexErros++;
                        }
                    }
                }

                /**
                 * O código da estratégia será validado pelas casas decimais antes do ponto.
                 * Exemplo: se uma meta possuir número igual a "20" e uma estratégia foi
                 * cadastrada com número igual a "20.1", a estratégia deve ser vinculada a meta.
                 *
                 * Se uma estratégia possuir número que não condiz com nenhuma meta cadastrada,
                 * o sistema não deve realizar a importação e o erro deve ser apresentado no
                 * relatório de erro.
                 */
                $arrAux = $arrTotal['metas'];
                $arrTotal['estrategias'][$key]['numeroMeta'] = substr($estrategia[0], 0, strpos($estrategia[0], '.'));

                if (!in_array($arrTotal['estrategias'][$key]['numeroMeta'], array_column($arrAux, 0))) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. Esta Estratégia não está vinculada a uma meta na aba Metas. Permitido: Nº da Meta.Nº da Estratégia.";
                    $indexErros++;
                }

                /**
                 * O campo Número da Estratégia não pode estar repetido
                 */
                $arrAux = $arrTotal['estrategias'];
                unset($arrAux[$key]);
                $array_column = array_column($arrAux, 0);

                if (in_array($estrategia[0], $array_column)) {
                    $array_keys = array_keys($array_column, $estrategia[0]);

                    foreach ($array_keys as $key2) {
                        if ($estrategia[0] === $array_column[$key2]) {
                            $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> A. O Nº da Estratégia não deve ser informado mais de uma vez.";
                            $indexErros++;
                        }
                    }

                }

                // Limpar a coluna 'Descrição da Estratégia'
                //$estrategia[1] = trim(iconv("UTF-8","ISO-8859-1",$estrategia[1])," \t\n\r\0\x0B\xA0");                

                
                /**
                 * O campo Descrição da Estratégia não pode estar vazio.
                 */
                if (strlen($estrategia[1]) == 0) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> B. A descrição da Estratégia não pode estar vazia.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Estratégia deve permitir até 5000 caracteres.
                 */
                if (strlen($estrategia[1]) > 5000) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> B. A quantidade de caracteres foi excedida. Permitido: 5000 caraceteres.";
                    $indexErros++;
                }

                /**
                 * O campo Descrição da Estratégia não pode estar repetido
                 */
                //if (in_array($estrategia[1], array_column($arrAux, 1))) {
                //    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> B. A descrição da Submeta não deve ser informada mais de uma vez.";
                //    $indexErros++;
                //}

                /**
                 * O campo "Prazo" deve ser igual ou maior que 2014 e igual ou menor que 2030
                 */
                if ($estrategia[2] < 2014 || $estrategia[2] > 2030) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> C. O ano informado *({$estrategia[2]})* no campo \"Prazo\" deve ser igual ou maior que 2014 e igual ou menor que 2030.";
                    $indexErros++;
                }

                /**
                 * O campo Previsões Orçamentárias não pode estar vazio.
                 */
                if (strlen($estrategia[3]) == 0) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> D. O campo Previsões Orçamentárias não pode estar vazio.";
                    $indexErros++;
                }

                /**
                 * O campo Previsões Orçamentárias deve permitir até 500 caracteres.
                 */
                if (strlen($estrategia[3]) > 500) {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> D. A quantidade de caracteres foi excedida. Permitido: 500 caracteres.";
                    $indexErros++;
                }

                /**
                 * Caso o campo Previsões Orçamentárias venha com os valores
                 * "nao contemplado" e "nao se aplica", atualiza o campo corretamente
                 */
                $campoPrevisao = strtolower(trim(removeAcentos($estrategia[3])));
                $fbeprevisao = $fbencontemplado = $fbenaplica = 'null';

                if ($campoPrevisao == 'nao contemplado') {
                    $fbencontemplado = 'true';
                } elseif ($campoPrevisao == 'nao se aplica') {
                    $fbenaplica = 'true';
                } else {
                    $fbeprevisao = "'$campoPrevisao'";
                }

                /**
                 * O campo "Alcançou Estratégia?" somente pode ter os valores "Sim" e "Não"
                 */
                $campoAlcancou = strtolower(trim(removeAcentos($estrategia[4])));
                if ($campoAlcancou != 'nao' && $campoAlcancou != 'sim') {
                    $erro[$indexErros] = "<b>Aba</b> Estratégias; <b>Linha</b> {$key}; <b>Coluna</b> D. Valor inválido no campo \"Alcançou Estratégia?\". Permitido: Sim ou Não.";
                    $indexErros++;
                }

                /**
                 * Concatenando Insert
                 */
                
                $estrategia[1] =  addslashes(trim($estrategia[1]));
                $estrategia[2] =  addslashes(trim($estrategia[2]));
                 
                $arrTotal['estrategias'][$key]['insert'] =
                    "(" . 'FBMID' . ",'" . $_POST['usucpf'] . "'
                    ,'" . $numeroEstrategia . "','" . $estrategia[1] . "'
                    ,'" . $estrategia[2] . "'
                    ," . ($campoAlcancou == 'nao' ? 'false' : 'true') . "
                    ," . $fbeprevisao . "
                    ," . $fbencontemplado . "
                    ," . $fbenaplica . "
                    ), ";

                /**
                 * Concatenando select for insert
                 */
                $arrTotal['estrategias'][$key]['selectInsert'] =
                    "('" . $numeroEstrategia . "','" . trim($estrategia[1]) . "'
                    ,'A','" . $key . "' ), ";

                $selectInsertEstrategias .= $arrTotal['estrategias'][$key]['selectInsert'];

                /**
                 * Concatenando Historico Estrategia
                 */

                $arrTotal['estrategias'][$key]['insertHistorico'] = "
                ('{$_SESSION['usucpf']}','CREATE','" . date('Y-m-d H:m:s') . "','{$_SESSION['usucpf']}'
                ,'{$numeroEstrategia}'," . ($campoAlcancou == 'nao' ? 'false' : 'true') . ",{$fbeprevisao},'{$estrategia[2]}'
                ,'" . trim($estrategia[1]) . "',{$fbencontemplado},{$fbenaplica },HFBMID,HFBSID ),";


            } //fim foreach

            $selectInsertEstrategias = substr($selectInsertEstrategias, 0, -2);
            $selectInsertEstrategias .= ' ) SELECT  distinct estrategia.fbenumero ,estrategia.fbedescricao, inserts.linha
				FROM sase.fichabestrategia estrategia
				JOIN inserts ON estrategia.fbedescricao = inserts.fbedescricao
                    AND estrategia.fbenumero = inserts.fbenumero
                    AND estrategia.fbestatus = inserts.fbestatus';


        } //fim if (is_array($arrTotal['estrategias']))
        else 
        {
            // não existe a aba Estratégias!
            $erro[$indexErros] = "<b>Aba</b> Estratégias: Este arquivo não possui uma aba estratégias!";
            $indexErros++;
        }

        /**
         * Verifica Planilha Vazia
         */
        if (count($arrTotal['metas']) + count($arrTotal['indicadores']) + count($arrTotal['estrategias']) == 0) {
            $return = json_encode(array('resultado' => 'erro', 'msg' => 'Esta planilha foi considerada vazia! <br><br>Verifique as abas Metas, Indricadores e Estrat&eacute;gias <br>no arquivo modelo para importa&ccedil;&atilde;o. <br><br>Para importa&ccedil;&atilde;o do Monitoramento de Metas, clique na op&ccedil;&atilde;o apropriada.'));
            return $return;
        }

        if (count($erro) > 0) {
            $return = json_encode(array('resultado' => 'erro_dado',
                                        'msg'       => $this->utf8_encode_recursive($erro)));

            return $return;
        }

        global $db;

        //VERIFICA NO BANCO SE EXISTEM METAS
        if (count($arrTotal['metas']) != 0 && $arrTotal['metas'] != false) {
            $arrAux = $db->carregar($selectInsertMetas);            
            if ($arrAux != false) {
                foreach ($arrAux as $value) {
                    $db->executar("UPDATE sase.fichabmeta SET  fbmstatus = 'I' WHERE fbmid = {$value['fbmid']}");
//                    $erro[$indexErros] = "<b>Aba</b> Metas; <b>Linha " . $value['linha'] . "</b> . A Meta já existe no banco.";
//                    $indexErros++;
                }
            }
        }

        if (count($erro) > 0) {
            $return = json_encode(array('resultado' => 'erro_dado',
                                        'msg'       => $this->utf8_encode_recursive($erro)));

            return $return;
        }

        //EXECUTA INSERTS
        try {
            //INSERE AS METAS
            if (count($arrTotal['metas']) != 0 && $arrTotal['metas'] != false) {

                $arrAux = $db->carregar($insertMetas);

                //ARRAY COM O FBMID DAS METAS INSERIDAS
                $arrMetasInseridas = array_column($arrAux, 'fbmnumero', 'fbmid');

                //PREPARA INSERT HISTORICO
                $countAuxiliar = 0;
                foreach ($arrTotal['metas'] as $key => $meta) {

                    $fbmid = $arrAux[$countAuxiliar]['fbmid'];
                    $arrTotal['metas'][$key]['insertHistorico'] = str_replace('HFBMID', $fbmid, $arrTotal['metas'][$key]['insertHistorico']);
                    $sqlHistoricoMeta .= $arrTotal['metas'][$key]['insertHistorico'];
                    $countAuxiliar++;
                }

                //INSERT HISTORICO

                $sqlHistoricoMeta = substr($sqlHistoricoMeta, 0, -1);

                $db->carregar($sqlHistoricoMeta);

            }

            if (count($arrTotal['indicadores']) != 0 && $arrTotal['indicadores'] != false) {
                //PREPARA SUBMETA
                $insertSubmetas = 'INSERT INTO sase.fichabsubmeta (
                fbmid
                ,usucpf
                ,fbsnumero
                ,fbsdescricao
                ,fbsprazo
                ,fbsaalcancou
                ) VALUES ';

                foreach ($arrTotal['indicadores'] as $key => $submeta) {
                    $fbmid = array_search($submeta['numeroMeta'], $arrMetasInseridas);
                    $arrTotal['indicadores'][$key]['insert'] = str_replace('FBMID', $fbmid, $arrTotal['indicadores'][$key]['insert']);
                    $insertSubmetas .= $arrTotal['indicadores'][$key]['insert'];
                }

                $insertSubmetas = substr($insertSubmetas, 0, -2) . ' RETURNING fbsid,fbsnumero,fbmid ';

                //INSERE SUBMETAS
                $arrAux = $db->carregar($insertSubmetas);

                //PREPARA INSERT HISTORICO SUBMETA

                $insertHistoricoSubmetas = 'INSERT INTO sase.fichabindicador_historico 
                                    (huscpf, husacao, husdtacao, usucpf, fbsnumero,fbsaalcancou,fbsprazo,fbsdescricao,fbsid,fbmid)
                                VALUES ';

                $countAuxiliar = 0;

                foreach ($arrTotal['indicadores'] as $key => $meta) {

                    $fbmid = $arrAux[$countAuxiliar]['fbmid'];
                    $fbsid = $arrAux[$countAuxiliar]['fbsid'];
                    $arrTotal['indicadores'][$key]['insertHistorico'] = str_replace('HFBMID', $fbmid, $arrTotal['indicadores'][$key]['insertHistorico']);
                    $arrTotal['indicadores'][$key]['insertHistorico'] = str_replace('HFBSID', $fbsid, $arrTotal['indicadores'][$key]['insertHistorico']);
                    $insertHistoricoSubmetas .= $arrTotal['indicadores'][$key]['insertHistorico'];
                    $countAuxiliar++;
                }

                $insertHistoricoSubmetas = substr($insertHistoricoSubmetas, 0, -1);

                $db->carregar($insertHistoricoSubmetas);

            }

            if (count($arrTotal['estrategias']) != 0 && $arrTotal['estrategias'] != false) {
                //PREPARA ESTRATEGIA
                $insertEstrategias = 'INSERT INTO sase.fichabestrategia (
                fbmid
                ,usucpf
                ,fbenumero
                ,fbedescricao
                ,fbeprazo
                ,fbealcancou
                ,fbeprevisao
                ,fbencontemplado
                ,fbenaplica
                ) VALUES ';

                foreach ($arrTotal['estrategias'] as $key => $estrategia) {
                    $fbmid = array_search($estrategia['numeroMeta'], $arrMetasInseridas);
                    $arrTotal['estrategias'][$key]['insert'] = str_replace('FBMID', $fbmid, $arrTotal['estrategias'][$key]['insert']);
                    $insertEstrategias .= $arrTotal['estrategias'][$key]['insert'];
                }

                $insertEstrategias = substr($insertEstrategias, 0, -2) . ' RETURNING fbeid,fbenumero, fbmid ';

                //INSERE ESTRATEGIA
                $arrAux = $db->carregar($insertEstrategias);

                //PREPARA INSERT HISTORICO ESTRATEGIA

                $insertHistoricoEstrategia = "INSERT INTO sase.fichabestrategia_historico 
                        (huscpf,husacao,husdtcriacao,usucpf,fbenumero,fbealcancou,fbeprevisao
                        ,fbeprazo, fbedescricao,fbencontemplado,fbenaplica, fbmid, fbeid ) VALUES ";

                $countAuxiliar = 0;

                foreach ($arrTotal['estrategias'] as $key => $estrategia) {

                    $fbmid = $arrAux[$countAuxiliar]['fbmid'];
                    $fbsid = $arrAux[$countAuxiliar]['fbeid'];
                    $arrTotal['estrategias'][$key]['insertHistorico'] = str_replace('HFBMID', $fbmid, $arrTotal['estrategias'][$key]['insertHistorico']);
                    $arrTotal['estrategias'][$key]['insertHistorico'] = str_replace('HFBSID', $fbsid, $arrTotal['estrategias'][$key]['insertHistorico']);
                    $insertHistoricoEstrategia .= $arrTotal['estrategias'][$key]['insertHistorico'];
                    $countAuxiliar++;
                }

                $insertHistoricoEstrategia = substr($insertHistoricoEstrategia, 0, -1);

                $db->carregar($insertHistoricoEstrategia);

            }
            
            //###arquivando os anexos da importação
            if (($_FILES['userfile']['size'] > 0) ){
                $campos          = array();
                $file            = new FilesSimec("arqimportacaofichab_historico", $campos, 'sase');
                if (preg_match('/[^\x20-\x7f]/', $_FILES['userfile']['name'])) {
                    $name = trim(($_FILES['userfile']['name']));
                    $a = 'ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýýþÿ???';
                    $b = 'aaaaaaaceeeeiiiidnoooooouuuuybsaaaaaaaceeeeiiiidnoooooouuuuyybyRr-';
                    $name = strtr($name, ($a), $b);
                    $name = preg_replace( "/[^0-9a-zA-Z\.]+/",'_',$name);
                }else {
                    $name = trim($_FILES['userfile']['name']);
                }
                
                
                $nomeArquivo     = $file->setUpload($name, 'userfile', false);
                $fmaid           = $_REQUEST['fmaid'] ? $_REQUEST['fmaid'] : 'null';
                $arqid           = $file->getIdArquivo();
                $sqlHistorico = "INSERT INTO sase.arqimportacaofichab_historico 
                                    (estuf, muncod, aifbhano, aifbhdtinclusao, aifbhstatus,usucpf,arqid, fmaid)
                                 VALUES ('{$_REQUEST['estuf']}',
                                         '{$_REQUEST['muncod']}',
                                          {$_SESSION['exercicio']},
                                         '" . date('Y-m-d H:m:s') . "',
                                         'A',
                                         '{$_SESSION['usucpf']}',
                                          {$arqid},
                                          {$fmaid}
                                    )";

                $i = $db->executar($sqlHistorico);
            }
            //### fim arquivando os anexos da importação


            $db->commit();
        } catch (Exception $e) {

            $db->rollback();
            echo 'false';
            die;
        }

        $return = json_encode(array('resultado' => 'success'));

        return $return;

    }

    /**
     * METODO PARA FAZER O UPLOAD DE INFORMAÇÕES DA FICHA C
     * @return bool
     */
    public function uploadFichaC()
    {
                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;  

        //VERIFICAR ERROS
        if ($this->getErroUpload() !== false) {

            $return = json_encode(array('resultado' => 'erro', 'msg' => $this->getErroUpload()));

            return $return;
        }
                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 1=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;  
        /**
         * O nome permitido das planilhas do arquivo recebido
         */

        $this->setArrSheetAllowed(array('monitoramento de metas'));

        $arrTotal = $this->carregarDadosPlanilha();

        $titulos = $this->getArrTitulos();
        $insert = false;
        $update = false;
  
                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 2=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;  
        
        //PREPARA PARA INSERIR

        $indexErros = 0;

        if (is_array($arrTotal['monitoramento de metas'])) {
                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 3=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;              
            $selectInsertMonitoramento = 'WITH inserts (
                fbmnumero
              , fbsnumero
              , muncod
              , estuf
              , fbsdescricao
              , fcmano
              , fcmmetaprevista
              , fcmmetaexecutadaoficial
              , fcmmetaexecutadaalternativa
              , linha
            )
            AS ( VALUES ';

            $insertMonitoramento = 'INSERT INTO sase.fichacmonitoramento (
                fbsid
                ,muncod
                ,estuf
                ,fcmano
                ,fcmmetaprevista
                ,fcmmetaexecutadaoficial
                ,fcmmetaexecutadaalternativa
                ) VALUES ';

            $updateMonitoramento = 'UPDATE sase.fichacmonitoramento AS m SET
                fcmmetaprevista = c.fcmmetaprevista,
                fcmmetaexecutadaoficial = c.fcmmetaexecutadaoficial,
                fcmmetaexecutadaalternativa = c.fcmmetaexecutadaalternativa
            FROM (VALUES  ';

            $arrNumeroAno = [];

                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 4=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;              
            foreach ($arrTotal['monitoramento de metas'] as $key => $indicador) {

                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 5=' 
                         //. $indicador[0] 
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;       
                /**
                 * Retira as linha em branco
                 */
                $countColunas = strlen(trim($indicador[0])) + strlen(trim($indicador[1]))
                    + strlen(trim($indicador[2])) + strlen(trim($indicador[3]))
                    + strlen(trim($indicador[4])) + strlen(trim($indicador[5]));
                if ($countColunas == 0) {
                    unset($arrTotal['monitoramento de metas'][$key]);
                    continue;
                }

                         //DEBUG
                         //$return = json_encode(array('resultado' => 'erro', 'msg' =>'debug 6=' 
                         //. $indicador[0]  
                         //. '<br>countColunas =' . $countColunas
                         //. '<br>Descricao =' . $indicador[1] 
                         // . '<br>Ano=' . $indicador[2] 
                         // . '<br>Meta Prevista=' . $indicador[3] 
                         // . '<br>Dado Oficial =' . $indicador[4] 
                         // . '<br>Dado Municipal=' . $indicador[5] 
                         // . '<br>QtdErros NovoT = ' . $indexErros 
                        //));
//                          return $return;
//                          die;       
                /**
                 * É necessário preencher ao menos um dos seguintes campos:
                 * "Meta Prevista", "Meta Executada no Período - Dado Oficial" e "Meta Executada no Período - Dado Municipal".
                 *
                 */
                $countMetas = strlen(trim($indicador[3])) + strlen(trim($indicador[4])) + strlen(trim($indicador[5]));
                if ($countMetas == 0) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}. É necessário preencher ao menos um dos seguintes campos: \"Meta Prevista\", \"Meta Executada no Período - Dado Oficial\" e \"Meta Executada no Período - Dado Municipal\".";
                    $indexErros++;
                    continue;
                }
                 //DEBUG
                 //$return = json_encode(array('resultado' => 'erro', 'msg' =>'Indicador =' . $indicador[0] 
                //. '<br>Descricao =' . $indicador[1] 
                // . '<br>Ano=' . $indicador[2] 
                // . '<br>countMetas =' . $countMetas
                // . '<br>Meta Prevista=' . $indicador[3] 
                // . '<br>Dado Oficial =' . $indicador[4] 
                // . '<br>Dado Municipal=' . $indicador[5] 
                // . '<br>QtdErros NovoT = ' . $indexErros ));
//                 return $return;
//                 die;                
                /*
                    Remover espaços em branco
                    Substituir virgula por ponto
                    Remover pontos consecutivos
                */
                $indicador[0] = $this->removeTodosEspacos($indicador[0]);                
                $indicador[0] = str_replace(',', '.', $indicador[0]);
                $indicador[0] = str_replace('..', '.', $indicador[0]);
                $indicador[0] = str_replace('..', '.', $indicador[0]);                
                 //DEBUG
                // $return = json_encode(array('resultado' => 'erro', 'msg' =>'Indicador =' . $indicador[0] 
                //. '<br>Descricao =' . $indicador[1] 
                // . '<br>Ano=' . $indicador[2] 
                 //. '<br>countMetas =' . $countMetas
                // . '<br>Meta Prevista=' . $indicador[3] 
               //  . '<br>Dado Oficial =' . $indicador[4] 
               //  . '<br>Dado Municipal=' . $indicador[5] 
               //  . '<br>QtdErros NovoT = ' . $indexErros ));
//                 return $return;
//                 die;  

                /**
                 * O campo "Número do Indicador" é obrigatório.
                 */
                if (strlen(trim($indicador[0])) == 0) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> A. O campo \"{$titulos[0]}\" é obrigatório.";
                    $indexErros++;
                }

                /**
                 * O campo "Número do Indicador" precisa estar no padrão Nº da Meta.Nº do Indicador.
                 */
                $arrNumIndicador = explode('.', $indicador[0]);
                if (strpos($indicador[0], '.') === false || !is_numeric($arrNumIndicador[0])) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> A. O campo \"{$titulos[0]}\" precisa estar no padrão: Nº da Meta.Nº do Indicador.";
                    $indexErros++;

                }

                 //DEBUG
                 //$return = json_encode(array('resultado' => 'erro', 'msg' =>'Indicador12 =' . $indicador[0] 
                //. '<br>Descricao =' . $indicador[1] 
                // . '<br>Ano=' . $indicador[2] 
                 //. '<br>countMetas =' . $countMetas
                 //. '<br>Meta Prevista=' . $indicador[3] 
                 //. '<br>Dado Oficial =' . $indicador[4] 
                 //. '<br>Dado Municipal=' . $indicador[5] 
                 //. '<br>QtdErros NovoT = ' . $indexErros ));
                 //return $return;
                 //die;  

                /**
                 * O campo "Descrição do Indicador" é obrigatório.
                 */
                if (strlen(trim($indicador[1])) == 0) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> B. O campo \"{$titulos[1]}\" é obrigatório.";
                    $indexErros++;
                }
               
                /**
                 * O campo Número do Indicador deve permitir até 20 caracteres, soma de 10 caracteres da meta + 10 caracteres da submeta (vugo indicador).
                 */
                if (strlen($indicador[0]) > 20) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> A. A quantidade de caracteres foi excedida. Permitido: 20 caracteres.";
                    $indexErros++;
                }
                // Limpar a coluna 'Ano'
                $indicador[2] = $this->retornaAnoPrazo($indicador[2]);
              
                /**
                 * O campo "Ano" deve ser igual ou maior que 2014 e igual ou menor que 2030
                 */
                if ($indicador[2] < 2014 || $indicador[2] > 2030) {
                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> C. O ano informado no campo \"{$titulos[2]}\" deve ser igual ou maior que 2014 e igual ou menor que 2030 ";
                    $indexErros++;
                }

                /**
                 * Validação dos campos: 
                 * Meta Prevista	
                 * Meta Executada no Período - Dado Oficial 	
                 * Meta Executada no Período - Dado Municipal
                 */

                 $indicador[3] = $this->IsNullOrEmptyString($indicador[3]) ? 'CAST(NULL AS NUMERIC)' : $this->mantemValor($indicador[3]);

                 $indicador[4] = $this->IsNullOrEmptyString($indicador[4]) ? 'CAST(NULL AS NUMERIC)' : $this->mantemValor($indicador[4]);

                 $indicador[5] = $this->IsNullOrEmptyString($indicador[5]) ? 'CAST(NULL AS NUMERIC)' : $this->mantemValor($indicador[5]);

                 if ($indicador[3] !=  'CAST(NULL AS NUMERIC)'){
                    $indicador[3] = $this->IsDecimal($indicador[3]) ? number_format($indicador[3], 2, '.', '') : number_format($indicador[3], 0, '.', '') ;
                    if ((float)$indicador[3] > 2000000) {
                        //O campo "Meta Prevista" não pode ser maior que 2000000
                        $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> D.  O valor \"{$indicador[3]}\" da coluna \"{$titulos[3]}\" não pode ser maior que 2000000";
                        $indexErros++;
                    }
    
                }
                 if ($indicador[4] !=  'CAST(NULL AS NUMERIC)'){
                    $indicador[4] = $indicador[4] !=  'CAST(NULL AS NUMERIC)' && $this->IsDecimal($indicador[4]) ? number_format($indicador[4], 2, '.', '') : number_format($indicador[4], 0, '.', '') ;
                    if ((float)$indicador[4] > 2000000) {
                        //O campo "Meta Prevista" não pode ser maior que 2000000
                        $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> E.  O valor \"{$indicador[4]}\" da coluna \"{$titulos[4]}\" não pode ser maior que 2000000";
                        $indexErros++;
                    }
                }
                 if ($indicador[5] !=  'CAST(NULL AS NUMERIC)'){
                    $indicador[5] = $indicador[5] !=  'CAST(NULL AS NUMERIC)' && $this->IsDecimal($indicador[5]) ? number_format($indicador[5], 2, '.', '') : number_format($indicador[5], 0, '.', '') ;
                    if ((float)$indicador[5] > 2000000) {
                        //O campo "Meta Prevista" não pode ser maior que 2000000
                        $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> F. O valor \"{$indicador[5]}\" da coluna \"{$titulos[5]}\" não pode ser maior que 2000000";
                        $indexErros++;
                    }
                }

//                 //DEBUG
//                 $return = json_encode(array('resultado' => 'erro', 'msg' =>'Indicador17 =' . $indicador[0] 
//                //. '<br>Descricao =' . $indicador[1] 
//                //. '<br>Descricao =' . $indicador[1] 
//                . '<br>Ano=' . $indicador[2] 
//                 //. '<br>countMetas =' . $countMetas
//                 . '<br>Meta Prevista=' . $indicador[3] 
//                 . '<br>Dado Oficial =' . $indicador[4] 
//                 . '<br>Dado Municipal=' . $indicador[5] 
//                 . '<br>QtdErros NovoT = ' . $indexErros ));
//                 return $return;
//                 die;  
  
//                echo "<pre>
//                <b>Arquivo:</b> UploadFichaMonitoramento.class.inc - <b>Linha:</b> 1098  
//                <b>Descrição5:</b>   <br>";
//                var_dump( $indicador[3] );
//                $indicador[3] = substr($indicador[3],0,5);
//                var_dump( $indicador[3] );
////                var_dump( $exploded );
////                var_dump( round ($exploded[1]) );
//
//                var_dump( round ($indicador[3],2) );
//                $teste = round ($indicador[3],2);
//                echo 'bla';
//
//                echo 'bla2';
//                var_dump(round($teste,2));

//                var_dump( ceil ($indicador[3]) );

//                var_dump( $erro );
//                echo "</pre>";
                
                
//                die;

                /**
                 * Concatenando select for insert
                 */
                $exploded = explode('.', $indicador[0],2);
 
                
                $arrTotal['monitoramento de metas'][$key]['selectInsert'] = "( " . $exploded[0] . ",'" . $exploded[1] . "', '" . $_POST['muncod'] . "','" . $_POST['estuf'] . "'
                    ,'" . $indicador[1] . "'," . $indicador[2] . "," . $indicador[3] . "," . $indicador[4] . ",
                    " . $indicador[5] . ", " . $key . "), ";

                $selectInsertMonitoramento .= $arrTotal['monitoramento de metas'][$key]['selectInsert'];

                /**
                 * Concatenando Insert
                 */
                $arrTotal['monitoramento de metas'][$key]['insert'] = "
                    ( FBSID , " . $_POST['muncod'] . ",'" . $_POST['estuf'] . "'
                    ," . $indicador[2] . "," . $indicador[3] . "," . $indicador[4] . "
                    ," . $indicador[5] . "), ";

                /**
                 * Concatenando Update
                 */
                $arrTotal['monitoramento de metas'][$key]['update'] = " 
                (FCMID,$indicador[3],$indicador[4],$indicador[5]), ";

                /**
                 * O campo Ano não pode estar repetido por Numero do Indicador
                 */
                $arrAux = $arrTotal['monitoramento de metas'];
                unset($arrAux[$key]);
                $arrayColumn = array_column($arrAux, 0);

                if (in_array($indicador[0], $arrayColumn) && strlen(trim($indicador[0])) != 0) {
                    $numeroAno = $indicador[0] . $indicador[2];

                    if (!in_array($numeroAno, $arrNumeroAno)) {
                        $arrNumeroAno[] = $numeroAno;
                        continue;
                    }

                    $erro[$indexErros] = "<b>Linha</b> {$key}; <b>Coluna</b> A. Os valores do indicador \"{$indicador[0]}\" e ano \"{$indicador[2]}\" est&atilde;o repetidos.";
                    $indexErros++;
                }
            } //fim foreach

            $updateMonitoramento = substr($updateMonitoramento, 0, -2);

            $aux = ($_POST['muncod'] == 'null') ? ' meta.muncod IS NULL ' : ' meta.muncod = inserts.muncod ';
            $selectInsertMonitoramento = substr($selectInsertMonitoramento, 0, -2);
            $selectInsertMonitoramento .= ' )
                    SELECT
                      inserts.linha, inserts.fbmnumero || \'.\' || inserts.fbsnumero as numeroindicador
                      ,meta.fbmid, sub.fbsid, fichac.fcmid,
                      CASE
                        WHEN (inserts.fcmano = fichac.fcmano) THEN \'update\'
                        ELSE \'insert\'
                      END AS acao,
                      CASE
                        WHEN (meta.fbmid ISNULL ) or (sub.fbsid ISNULL ) THEN \'sim\'
                        ELSE \'nao\'
                      END AS temErro
                    
                    FROM inserts
                      LEFT JOIN sase.fichabmeta meta ON meta.fbmnumero = inserts.fbmnumero
                                                        AND meta.estuf = inserts.estuf
                                                        AND meta.fbmstatus = \'A\'
                                                        AND ' . $aux . ' 
                      LEFT JOIN sase.fichabsubmeta sub ON sub.fbsnumero = inserts.fbsnumero
                                                          AND inserts.fbsdescricao = sub.fbsdescricao
                                                          AND sub.fbmid = meta.fbmid
                                                          AND sub.fbsstatus = \'A\'
                      LEFT JOIN sase.fichacmonitoramento fichac ON sub.fbsid = fichac.fbsid
                                                                   AND inserts.fcmano = fichac.fcmano ';

        } // fim if (is_array($arrTotal['metas']))
        

        if ($arrTotal['monitoramento de metas'] == false) {

            $return = json_encode(array('resultado' => 'erro', 'msg' => 'Esta planilha foi considerada vazia! <br><br>Verifique a exist&ecirc;ncia da aba Monitoramento de Metas <br>no arquivo modelo para importa&ccedil;&atilde;o. <br><br>Para importa&ccedil;&atilde;o das Metas e Estrat&eacute;gias, clique na op&ccedil;&atilde;o apropriada.'));

            return $return;
        }

        if (count($erro) > 0) {

            $return = json_encode(array('resultado' => 'erro_dado',
                                        'msg'       => $this->utf8_encode_recursive($erro)));

            return $return;
        }

        global $db;

        //VERIFICA NO BANCO SE EXISTEM MONITORAMENTOS

        if (count($arrTotal['monitoramento de metas']) != 0 && $arrTotal['monitoramento de metas'] != false) {

            $arrAux = $db->carregar($selectInsertMonitoramento);

            foreach ($arrAux as $index => $inserts) {

                /*
                 * insere na query id do indicador
                 */
                if ($inserts['acao'] == 'insert') {
                    $insert = true;
                    $arrTotal['monitoramento de metas'][$inserts['linha']]['insert'] =
                        str_replace('FBSID', $inserts['fbsid'], $arrTotal['monitoramento de metas'][$inserts['linha']]['insert']);

                    $insertMonitoramento .= $arrTotal['monitoramento de metas'][$inserts['linha']]['insert'];
                }

                if ($inserts['acao'] == 'update') {
                    $update = true;
                    $arrTotal['monitoramento de metas'][$inserts['linha']]['update'] =
                        str_replace('FCMID', $inserts['fcmid'], $arrTotal['monitoramento de metas'][$inserts['linha']]['update']);

                    $updateMonitoramento .= $arrTotal['monitoramento de metas'][$inserts['linha']]['update'];
                }

                if ($inserts['temerro'] == 'sim') {


                    $insert = true;
                    //$erro[$indexErros] = "<b>Linha *" . $selectInsertMonitoramento . "*" . $inserts['linha'] . "</b>. O \"Indicador\" não existe no banco.";
                    $erro[$indexErros] = "<b>Linha " . $inserts['linha'] . "</b>. O \"Indicador\" não existe no banco. Certifique-se de que o \"<b>Número do Indicador</b>\" e a \"<b>Descrição do indicador</b>\" estão idênticos aos valores cadastrados anteriormente em \"<b>Metas / Estratégias</b>\".";
                    $indexErros++;
                }
            }

            $insertMonitoramento = substr($insertMonitoramento, 0, -2) . ' RETURNING fcmid ';
            $updateMonitoramento = substr($updateMonitoramento, 0, -2) . ' ) as 
                c(fcmid, fcmmetaprevista, fcmmetaexecutadaoficial,fcmmetaexecutadaalternativa)
                where c.fcmid = m.fcmid returning m.fcmid';
        }

        if (count($erro) > 0) {
            $return = json_encode(array('resultado' => 'erro_dado',
                                        'msg'       => $this->utf8_encode_recursive($erro)));

            return $return;
        }

        //EXECUTA INSERTS
        try {

            //INSERE AS METAS

            $arrAuxInsert = ($insert) ? $db->carregar($insertMonitoramento) : false;
            $arrAuxUpdate = ($update) ? $db->carregar($updateMonitoramento) : false;

            $db->commit();

        } catch (Exception $e) {

            $db->rollback();
            echo 'false';
            die;
        }

        $return = json_encode(array('resultado' => 'success'));

        return $return;

    }

    function utf8_encode_recursive($array)
    {
        $utf8_array = array();
        foreach ($array as $key => $val) {
            if (is_array($val)) $utf8_array[$key] = utf8_encode_recursive($val); else $utf8_array[$key] = ($val);
        }

        return $utf8_array;
    }

}
