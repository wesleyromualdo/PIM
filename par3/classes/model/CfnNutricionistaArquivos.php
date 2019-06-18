<?php
/**
 * Classe de mapeamento da entidade par3.cfn_nutricionista_arquivos.
 *
 * @version $Id$
 * @since 2018.05.08
 */

/**
 * Par3_Model_CfnNutricionistaArquivos: Tabela utilizada para manter referência dos arquivos de carga para atualizar a
 * situação cadastral dos nutricionistas no sistema par3
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Cfn_nutricionista_arquivos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Cfn_nutricionista_arquivos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $cfnid Chave Primária - default: nextval('par3.cfn_nutricionista_arquivos_cfnid_seq'::regclass)
 * @property int $arqid FK para a tabela public.arquivo
 * @property string $cfnobservacao Informação complementar sobre o arquivo cfn
 * @property string $cfncpfinclusao CPF do usuário que incluiu o registro
 * @property \Datetime(Y-m-d H:i:s) $cfndtinclusao Data de inclusão do registro - default: now()
 * @property int $cfntotalregistros Total de registros processados no arquivo
 * @property int $cfndtemailenviado Validação para verificar se já foram enviados os e-mails após o processamento da
 * carga
 */
class Par3_Model_CfnNutricionistaArquivos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.cfn_nutricionista_arquivos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cfnid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'cfnid'             => null,
        'arqid'             => null,
        'cfnobservacao'     => null,
        'cfncpfinclusao'    => null,
        'cfndtinclusao'     => null,
        'cfntotalregistros' => null,
        'cfndtemailenviado' => null,
    );

    /**
     * Função para processar a carga CFN
     * @param $arqid
     */
    public function processarCargaCfn($cfnid, $arqid, $delimitador = null)
    {
        if (!$delimitador) {
            $delimitador = ';';
        } else {
            $delimitador = ($delimitador == 'P'?';':',');
        }
        //As posições dos índices  (começando em 0) corresponde às seguintes colunas:
        //UF,Municípios,Esfera,CPF,Nome,CRN,CRN-UF,Cargo/função,Vínculo,Situação,,Validação CFN,
        //Modelo de Validação
        //CFN,Registro/CRN,Situação Problema
        $getUnuid = function ($estuf, $mundsc = null) {
            if ($mundsc == 'Embu das Artes' && $estuf == 'SP') {//tratar divergência entre nomes das entidades
                $mundsc = 'Embu';
            }
            if ($estuf == 'DF') {
                return $this->pegaLinha(
                    "select inuid from par3.instrumentounidade where estuf = 'DF' and itrid = 3"
                )['inuid'];
            }
            $where = "where estuf = '{$estuf}' and itrid = 1";
            if ($mundsc) {
                $where = "where estuf = '{$estuf}' and itrid = 2 and inudescricao = '{$mundsc}'";
            }
            return $this->pegaLinha("select inuid from par3.instrumentounidade {$where}")['inuid'];
        };

        $getSnidByDsc = function ($sndescricao) {
            if (!$sndescricao) {
                return '';
            }
            return $this->pegaLinha(
                "select snid from par3.situacaonutricionista where sndescricao = '{$sndescricao}' "
            )['snid'];
        };

        $trataCPF = function ($cpf) {
            return str_repeat('0', (11 - mb_strlen($cpf))).$cpf;
        };

        $file       = new FilesSimec(null, null, "par3");
        $fileName   = $file->getCaminhoFisicoArquivo($arqid);
        $row = 0;
        $primeiraLinha = false;
        $arrVn              = [];
        $arrVnidAtualizados = [];
        $mvn = new Par3_Model_VinculacaoNutricionista();
        if (($handle = fopen($fileName, "r")) !== false) {
            while (($data = fgetcsv($handle, 10000, $delimitador)) !== false) {
                //pular cabeçalho
                if (!$primeiraLinha) {
                    $primeiraLinha = true;
                    continue;
                }
                $cpf    = $trataCPF($data[3]);
                $mundsc = $data[1];
                $inuid  = $getUnuid($data[0], addslashes(trim($mundsc)));
                if (!$inuid) {
                    continue;
                }
                $vnid = $mvn->carregarVinculacaoNutricionistaPorVnidInuid($cpf, $inuid, ['inuid','vnstatus'], true);
                if ($vnid['vnid'] && $vnid['vnstatus'] == 'A') {//Verifica se o nutricionista está vinculado
                    $snid = $getSnidByDsc($data[9]);

                    $arrVn['snid'] = 5;
                    if ($data[10] == 'Válido') {//atualiza a situação para aprovado
                        $arrVn['snid'] = 4;
                    }
                    if ($data[10] == 'Pendente') {//atualiza situação para pendente
                        $arrVn['snid'] = 3;
                    }
                    $arrVn['vnid'] = $vnid['vnid'];
                    $mvn->popularDadosObjeto($arrVn);
                    $vnidsalvo = $mvn->salvar();
                    $mvn->commit();
                    $arrVn = [];
                    $vnid['cfvdscmotivo'] = $data[13];
                    $vnid['cfvprocessado'] = "true";
                    $arrVnidAtualizados[] = $vnid;
                    $row++;
                    continue;
                }
                if ($vnid['vnid']) {
                    //Não atualiza o registro mas registra o nutricionista desvinculado
                    $vnid['cfvprocessado'] = "false";
                    $vnid['cfvdscmotivo'] = $data[13];
                    $arrVnidAtualizados[]  = $vnid;
                }
            }
            fclose($handle);
            $mcfnvn = new Par3_Model_CfnNutricionistaArquivosVnid();
            $mcfnvn->salvarNutricionistasAtualizados($cfnid, $arrVnidAtualizados);
        }
        $this->popularDadosObjeto(['cfnid' => $cfnid,'cfntotalregistros' => $row]);
        $this->salvar();
        $this->commit();
        return true;
    }

    public function validarCargaCfn($arqid, $delimitador)
    {
        if (!$delimitador) {
            $delimitador = ';';
        } else {
            $delimitador = ($delimitador == 'P'?';':',');
        }
        $file       = new FilesSimec(null, null, "par3");
        $fileName   = $file->getCaminhoFisicoArquivo($arqid);
        $primeiraLinha = false;
        $mvn = new Par3_Model_VinculacaoNutricionista();
        if (($handle = fopen($fileName, "r")) !== false) {
            while (($data = fgetcsv($handle, 10000, $delimitador)) !== false) {
                //pular cabeçalho
                if (!$primeiraLinha) {
                    $primeiraLinha = true;
                    continue;
                }
                if (count($data) < 14) {
                    return false;
                }
            }
            fclose($handle);
        }
        return true;
    }
    /**
     * Função para processar a carga CFN
     * @param $arqid
     */
    public function processarCargaCfnNovo($cfnid, $arqid)
    {
        $trataCPF = function ($cpf) {
            return str_repeat('0', (11 - mb_strlen($cpf))).$cpf;
        };

        $file       = new FilesSimec(null, null, "par3");
        $fileName   = $file->getCaminhoFisicoArquivo($arqid);
        $row = 0;
        $primeiraLinha = false;
        $arrVn = [];
        $arrVnidAtualizados = [];
        $mvn = new Par3_Model_VinculacaoNutricionista();
        if (($handle = fopen($fileName, "r")) !== false) {
            while (($data = fgetcsv($handle, 10000, ",")) !== false) {
                //pular cabeçalho
                if (!$primeiraLinha) {
                    $primeiraLinha = true;
                    continue;
                }
                $cpf    = $trataCPF($data[3]);
                $vnid = $mvn->carregarVinculacaoNutricionistaPorVnidInuid($cpf, $data[1], ['inuid']);

                if ($vnid['vnid']) {//Verifica se o nutricionista está vinculado
                    $snid = $data[11];

                    $arrVn['snid'] = 5;
                    if (($data[12]) == 'Válido') {//atualiza a situação para aprovado
                        $arrVn['snid'] = 4;
                    }
                    if (($data[12]) == 'Pendente') {//atualiza situação para pendente
                        $arrVn['snid'] = 3;
                    }
                    $arrVn['vnid'] = $vnid['vnid'];
                    $mvn->popularDadosObjeto($arrVn);
                    $vnidsalvo = $mvn->salvar();
                    $mvn->commit();
                    $arrVn = [];
                    $arrVnidAtualizados[] = $vnid;
                    $row++;
                }
            }
            fclose($handle);
            $mcfnvn = new Par3_Model_CfnNutricionistaArquivosVnid();
            $mcfnvn->salvarNutricionistasAtualizados($cfnid, $arrVnidAtualizados);
        }
        $this->popularDadosObjeto(['cfnid' => $cfnid,'cfntotalregistros' => $row]);
        $this->salvar();
        $this->commit();
    }

    public function validarInput($arrPost)
    {
        $arrayErros['erros']['arqid'] = array();
        $errosArqid = null;
        $primeiraLinha = false;
        if ($_FILES['arqid']) {
            $file = $_FILES["arqid"]["tmp_name"];
            if (($handle = fopen($file, "r")) !== false) {
                while (($data = fgetcsv($handle, 10000, $arrPost['delimitador'])) !== false) {
                    if (!$primeiraLinha) {
                        $primeiraLinha = true;
                        continue;
                    }
                    //Validar se o formato do arquivo está correto
                    if (count($data) < 1) {
                        fclose($handle);
                        $errosArqid .= 'O arquivo não está com os caracteres delimitadores corretos.';
                        unlink($_FILES["arqid"]["tmp_name"]);
                        break;
                    }
                }
            }

            if ($_FILES['arqid']['error'] != 0) {
                $errosArqid .= 'Ocorreu um erro.';
            }
            if (!in_array($_FILES['arqid']['type'], ['text/csv','application/vnd.ms-excel'])) {
                $errosArqid .= 'O arquivo está em um formato inválido. Formatos permitidos: (.csv).';
            }
            if (($_FILES['arqid']['size'] / 1000) > 10000) {
                $errosArqid .= 'Arquivo muito grande. tamanho máximo permitido: 10 mb';
            }
        }
        if (!$_FILES['arqid']) {
            $errosArqid .= 'Arquivo obrigatório';
        }

        $arrayErros['erros']['arqid'] = $errosArqid;

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($arrayErros['erros'] as $key => $value) {
            if (!empty($arrayErros['erros'][$key])) {
                return $arrayErros;
            }
        }
        return false;
    }

    public function getSqlListagemAjax($limit = null, $offset = null, $order = null, $dir = null)
    {
        $limitVal  = $limit?" LIMIT   {$limit} ":'';
        $offsetVal = $offset?" OFFSET {$offset} ":'';
        $orderBy   = $order?" ORDER BY ".($order + 1)." {$dir} ":" ORDER BY 4 desc ";
        $sql = "select 
                arq.arqid as arqid,
                arq.arqnome||'.'||arq.arqextensao as arquivo,
                cfn.cfnobservacao,
                TO_CHAR(cfn.cfndtinclusao,'DD/mm/YYYY HH24:MI:SS'),
                substr(usu.usucpf, 1, 3) || '.' ||
                substr(usu.usucpf, 4, 3) || '.' ||
                substr(usu.usucpf, 7, 3) || '-' ||
                substr(usu.usucpf, 10)||' - '||usu.usunome as usunome,
                cfn.cfntotalregistros as totalregistros,
                (CASE WHEN 
                    count(case when cfv.cfvprocessado = false then 1 end ) > 0 then cfn.cfnid
                END)as naoatualizados,
                count(*) OVER() AS full_count
            from par3.cfn_nutricionista_arquivos cfn
            inner join public.arquivo arq on arq.arqid = cfn.arqid
            inner join seguranca.usuario usu on usu.usucpf = cfn.cfncpfinclusao
            left  join par3.cfn_nutricionista_arquivos_vnid cfv on cfv.cfnid = cfn.cfnid
            group by arq.arqid,arquivo,usunome,totalregistros,cfnobservacao,cfn.cfndtinclusao,usu.usucpf,cfn.cfnid
            ";
        $res = $this->carregar($sql.$orderBy.$offsetVal.$limitVal);
        if (count($res) == 0) {
            $res = $this->carregar($sql.$orderBy.$limitVal);
        }
        return $res;
    }

    public function listarNutricionistasAtualizadosCFN($inuid, $vnid = null)
    {
        $sql = "SELECT vn.vnid,usu.usucpf,usu.usunome,sn.sndescricao,vn.inuid
                from par3.vinculacaonutricionista vn
                inner join par3.situacaonutricionista sn on sn.snid = vn.snid
                inner join seguranca.usuario usu on usu.usucpf = vn.vncpf
                inner join par3.dadosnutricionista usu on usu.usucpf = vn.vncpf
                where vn.vndatadesvinculacao is null and vn.vnstatus = 'A'
                and vn.inuid = {$inuid} and vnid = {$vnid}
        ";
        return $this->carregar($sql);
    }

    public function recuperarCargasSemEmailEnviado()
    {
        return $this->recuperarTodos('*', array('cfndtemailenviado is null'));
    }

    public function atualizarStatusEmailEnviadoCFN($cfnid)
    {
        $this->popularDadosObjeto(['cfnid' => $cfnid,'cfndtemailenviado' => date('Y-m-d H:m:s')]);
        $this->salvar();
        $this->commit();
    }
    public function recuperarNutricionistasAtualizadosPorCarga($cfnid, $inuid = null)
    {
        if (!$cfnid) {
            return array();
        }

        $whereUnuid = '';
        $arrClausulas = ["cfnid = {$cfnid}"];
        if ($inuid != '') {
            $whereUnuid = "and ent.inuid = {$inuid}";
            $arrClausulas[] ="inuid = {$inuid}";
        }
        $sql = "
                select vn.vncpf,ent.entnome,sn.sndescricao,ent.inuid,sn.snid,cfn_vn.cfvdscmotivo,usu.usuemail
                from par3.vinculacaonutricionista vn
                inner join par3.instrumentounidade_entidade     ent    on ent.entcpf  = vn.vncpf  
                  AND vn.inuid = ent.inuid 
                  AND   vn.vnstatus = 'A'
                inner join seguranca.usuario                    usu    on ent.entcpf  = usu.usucpf
                inner join par3.dadosnutricionista              dan    on dan.dancpf  = vn.vncpf
                inner join par3.situacaonutricionista           sn     on sn.snid     = vn.snid
                inner join par3.cfn_nutricionista_arquivos_vnid cfn_vn on cfn_vn.vnid = vn.vnid 
                where vn.vnid in(select vnid from par3.cfn_nutricionista_arquivos_vnid where cfnid = {$cfnid}) 
                and ent.entstatus = 'A' {$whereUnuid}
                and cfn_vn.cfnid = {$cfnid}
                group by vn.vncpf,ent.entnome,sn.sndescricao,ent.inuid,sn.snid,cfn_vn.cfvdscmotivo,usu.usuemail
        ";
//        ver($sql);
        return $this->carregar($sql);
    }

    public function recuperarInuidNutricionistasAtualizadosPorCfnid($cfnid)
    {
        if (!$cfnid) {
            return array();
        }
        $sql = "
                select inuid
                from par3.cfn_nutricionista_arquivos_vnid where cfnid = {$cfnid}
                group by inuid
        ";
        return array_map(function ($cfnvn) {
            return $cfnvn['inuid'];
        }, $this->carregar($sql));
    }


    public function recuperarNutricionistasAtualizadosPorInuidCfnid($cfnid, $inuid)
    {
        if (!$inuid || !$cfnid) {
            return array();
        }
        $sql = "
                select inuid
                from par3.cfn_nutricionista_arquivos_vnid where inuid = {$inuid} and cfnid = {$cfnid}
        ";
        return array_map(function ($cfnvn) {
            return $cfnvn['inuid'];
        }, $this->carregar($sql));
    }

    /**
     * Recupera os dados do dirigente ou secretário estadual de acordo com o instrumento-unidade
     * @param $inuid
     */
    public function recuperarDiretoria($inuid)
    {
        $mInu  = new Par3_Model_InstrumentoUnidade($inuid);
        $itrid = $mInu->itrid == 2?'= 2':'IN(1,3)';
        $tenid = $mInu->itrid == 2 ?
            Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE :
            Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
        $sql = "
        select ent.entemail,ent.entnome from par3.instrumentounidade inu
        inner join par3.instrumentounidade_entidade ent on ent.inuid = inu.inuid
        where ent.tenid   = {$tenid}
        and ent.inuid     = {$inuid}
        and ent.entstatus = 'A'
        and inu.itrid {$itrid}
        ";
        return $this->recuperar($sql);
    }

    public function enviarEmailDirigentes($entidade, $arrVn)
    {
        $strEmailTo = strtolower($entidade['entemail']);
        $email = 'daniel.fiuza@mec.gov.br';
        $file_content = '';
        ob_start();
        include APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/template_nutricionistas_por_entidade.php';
        $file_content = ob_get_contents();
        ob_end_clean();

        $strAssunto  = "Aviso cadastro de nutricionista";
        $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
        $strMensagem = $this->templateEmailGestor($file_content);

        if ($_SERVER['HTTP_HOST'] == "simec-dsv") {
            echo $strMensagem;
            return;
        }
        if ($_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
            $strEmailTo  = array('daniel.fiuza@mec.gov.br');
            $strAssunto  = 'teste e-mails de carga';
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $res= enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            return $res;
        }
        $res= enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
    }

    public function templateEmailGestor($listaNutricionistas)
    {
        $strTemplate = <<<EMAIL
    <pre align="center" style="text-align: justify;">
    Prezado(a) gestor(a), segue a situação do cadastro no Simec dos nutricionistas de seu município:
    <pre align="center" style="text-align: justify;margin-left:110px;">
        {$listaNutricionistas}
    </pre>
    Caso haja cadastro reprovado, o nutricionista deverá entrar em contato com o
    CRN a que está vinculado para regularizar sua situação.
    <br>
    Atenciosamente,
    <br/>
    Coordenação de Segurança Alimentar e Nutricional
  
    <span style="color:#0a6aa1;">Em caso de dúvidas, entrar em contato pelo PAR Fale Conosco,
    no endereço <a href="http://www.fnde.gov.br/parfaleconosco/index.php/publico" target="_blank">www.fnde.gov.br/parfaleconosco/index.php/publico</a>
    ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
    </span>
    </pre>
EMAIL;
        return $strTemplate;
    }

    public function enviarEmailNutricionistaPosCFN($arrDados)
    {

        //Dados padrão para envio de email para os nutricionistas
        $motivoReprovacao = $arrDados['cfvdscmotivo']?'por '.$arrDados['cfvdscmotivo']:'';
        $strEmailTo  = $nutricionista['usuemail'];
        $strAssunto  = 'Atualização cadastro de Nutricionista';
        $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

        $rodape = <<<RODAPE
    <span style="color:#0a6aa1;">
    Em caso de dúvidas, entrar em contato pelo PAR Fale Conosco,
    no endereço <a href="http://www.fnde.gov.br/parfaleconosco/index.php/publico" target="_blank">www.fnde.gov.br/parfaleconosco/index.php/publico</a>
    ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
    </span>
RODAPE;
        switch ($arrDados['snid']) {
            case 4:
                $mInu = new Par3_Model_InstrumentoUnidade($arrDados['inuid']);
                $descricao = ($mInu->itrid == 2?'Município de '.$mInu->inudescricao.'-'.$mInu->estuf : 'Estado de '.$mInu->inudescricao);
                $nomeMunicipio = "";
                $strMensagem = <<<EMAIL
    <pre align="center" style="text-align: justify;">
    Prezado(a) Nutricionista, 
    
    seu cadastro no Simec foi concluído com sucesso para o {$descricao} {$nomeMunicipio}.

    Atenciosamente,
    
    Coordenação de Segurança Alimentar e Nutricional
    
    {$rodape}
    </pre>
EMAIL;
                break;
            case 5:
                $strMensagem = <<<EMAIL
    <pre align=\"center\" style=\"text-align: justify;\">
    Prezado(a) Nutricionista,
    
    seu cadastro no Simec não foi aprovado.
    Entre em contato com o CRN a que está vinculado para regularizar sua situação.


    Atenciosamente,
    
    Coordenação de Segurança Alimentar e Nutricional
    </pre>
EMAIL;
                break;
            case 3:
                $strMensagem = <<<EMAIL
    <pre align=\"center\" style=\"text-align: justify;\">
    Prezado(a) Nutricionista,
    
    seu cadastro no Simec não foi aprovado.
    Entre em contato com o CRN a que está vinculado para regularizar sua situação.


    Atenciosamente,
    
    Coordenação de Segurança Alimentar e Nutricional
    </pre>
EMAIL;
                break;
        }

        if ($_SERVER['HTTP_HOST'] == "simec-dsv") {
            echo $strMensagem;
            return;
        }
        if ($_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
            $strEmailTo  = array('daniel.fiuza@mec.gov.br');
            $strAssunto  = 'teste e-mails de carga';
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            return $retorno;
        }
        $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        return $strMensagem;
    }

    public function carregarNutricionistasNaoAtualizados($cfnid)
    {
        if (!$cfnid) {
            return null;
        }

        $sql ="
        select DISTINCT 
            inu.estuf,
            inu.inudescricao,
            vn.vncpf,
            usu.usunome,
            (case when vn.vnstatus = 'I' then 'Inativo' else 'Ativo' end) as vnstatus,
            vn.vndatadesvinculacao,
            vn.vnmotivodesvinculacao
        from par3.cfn_nutricionista_arquivos_vnid cfv
        inner join par3.vinculacaonutricionista           vn on vn.vnid = cfv.vnid      and cfv.inuid = vn.inuid
        inner join seguranca.usuario                     usu on usu.usucpf = vn.vncpf
        inner join par3.instrumentounidade               inu on inu.inuid   = vn.inuid
        inner join par3.instrumentounidade_entidade     ent  on ent.entcpf  = vn.vncpf  AND vn.inuid = ent.inuid 
        inner join par3.situacaonutricionista            sn  on sn.snid     = vn.snid
        where cfv.cfnid = {$cfnid}
        and cfv.cfvprocessado = false
        order by inu.estuf
        ";
        return $this->carregar($sql);
    }
}
