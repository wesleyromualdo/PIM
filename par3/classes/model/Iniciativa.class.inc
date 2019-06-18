<?php

/**
 * Classe de mapeamento da entidade par3.iniciativa.
 *
 * @version $Id$
 * @since 2017.06.23
 */

/**
 * Par3_Model_Iniciativa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 *
 * @property int $iniid  - default: nextval('par3.iniciativa_iniid_seq'::regclass)
 * @property "char" $inistatus Status da iniciativa
 * @property "char" $inisituacao
 * @property int $indid  - default: nextval('par3.iniciativa_indid_seq'::regclass)
 * @property int $intaid FK do Tipo de Atendimento (par3.iniciativa_tipos_atendimento) - default: nextval('par3.iniciativa_intaid_seq'::regclass)
 * @property int $cicid FK do Ciclo (par3.ciclo_par) - default: nextval('par3.iniciativa_cicid_seq'::regclass)
 * @property int $intoid FK do Tipo de Objeto (par3.iniciativa_tipos_objeto) - default: nextval('par3.iniciativa_intoid_seq'::regclass)
 * @property "char" $iniesfera Esfera da iniciativa. "M" => Municipal, "E" => Estadual, "T" => Todos
 * @property string $initodos_estuf S ou N. Indica se possui todos os estados
 * @property string $initodos_muncod S ou N. Indica se possui todos os municípios
 */
class Par3_Model_Iniciativa extends Modelo
{

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iniid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'intoid' => array('tabela' => 'par3.iniciativa_tipos_objeto', 'pk' => 'intoid'),
        'cicid' => array('tabela' => 'par3.ciclo_par', 'pk' => 'cicid'),
        'intaid' => array('tabela' => 'par3.iniciativa_tipos_atendimento', 'pk' => 'intaid'),
        'indid' => array('tabela' => 'par3.iniciativa_descricao', 'pk' => 'indid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iniid' => null,
        'inistatus' => null,
        'inisituacao' => null,
        'indid' => null,
        'intaid' => null,
        'cicid' => null,
        'intoid' => null,
        'iniesfera' => null,
        'initodos_estuf' => null,
        'initodos_muncod' => null,
        'inivalidapendencia' => null,
        'iniobraproinfancia' => null,
    );
    const WORKFLOW_TIPO_DOCUMENTO_INICIATIVA = 302;

    public function getIniciativa(array $arrPost)
    {
        $where = $arrPost['iniid'] ? " AND iniid = {$arrPost['iniid']}" : '';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE inistatus = 'A' {$where}";
        //die($sql);
        return $this->pegaLinha($sql, 0, 0);
    }

    public function sqlAnosIniciativaCombo($iniid)
    {
        if ($iniid) {
            $sql = "SELECT
                        iniano as codigo,
                        iniano as descricao
                    FROM par3.iniciativa_iniciativas_anos
                    WHERE
                        iianostatus = 'A'
                        AND iniid = $iniid";
        } else {
            $sql = "SELECT 'na' as codigo, 'Escolha uma iniciativa' as descricao;";
        }

        return $sql;
    }

    public function listarSelectDimensoesIniciativas($arrPost = null, $consulta = false)
    {

        if ($consulta) {
            $where = "  WHERE inistatus IN ('A','I') ";
        } else {
            $where = " WHERE inistatus = 'A' AND   inisituacao = 'A' ";
        }

        $where .= $arrPost['iniid'] ? " AND ini.iniid     = '{$arrPost['iniid']}'" : '';
        $where .= $arrPost['dimid'] ? " AND idi.dimid     =  {$arrPost['dimid']}" : '';
        $innerJoin = '';
        $andOr = "";


        if ($arrPost['iniesfera'] == 'E') {
            $andOr = "OR (iniesfera IN('E','T') AND (iiestuf.estuf = '{$arrPost['estuf']}' OR initodos_estuf  = 'S') )";
        }

        if ($arrPost['iniesfera'] == 'M') {
            $andOr = "OR ( iniesfera IN('M','T') AND (iimun.muncod = '{$arrPost['muncod']}' OR initodos_muncod = 'S') )";
        }

        if ($arrPost['dimid']) {
            $innerJoin = " INNER JOIN par3.iniciativa_indicadores as idi ON idi.iniid = ini.iniid  AND idistatus = 'A'";
        }

        if ($arrPost['iniesfera'] == 'E') {
            $innerJoin .= " LEFT JOIN par3.iniciativa_iniciativas_estados as iiestuf ON iiestuf.iniid = ini.iniid";
        }

        if ($arrPost['iniesfera'] == 'M') {
            $innerJoin .= " LEFT JOIN par3.iniciativa_iniciativas_municipios as iimun   ON iimun.iniid   = ini.iniid";
        }

        if($_GET['obrid'] > 0 && !$consulta) {
            $where .= ' AND iniobraproinfancia IS TRUE ';
        }else {
            $where .= ' AND iniobraproinfancia IS NOT TRUE ';
        }

        $sql = "
            SELECT ini.iniid as codigo, (ini.iniid||' - '||ind.inddsc) as descricao
            FROM {$this->stNomeTabela} ini
            INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
            {$innerJoin}
            {$where}
            AND(iniesfera = 'T'
                {$andOr}
            )
            ORDER BY ini.iniid";
//        ob_clean();
//        ver($sql,$arrPost,d);
        $resultado = $this->carregar($sql);
        return $resultado ? simec_preparar_array($resultado) : array();
    }

    public function listarSelectDimensoesIniciativasInp($arrPost = null, $consulta = false)
    {

        if ($consulta) {
            $where = "  WHERE inistatus IN ('A','I') ";
        } else {
            $where = " WHERE inistatus = 'A' AND   inisituacao = 'A' ";
        }

        $where .= $arrPost['iniid'] ? " AND ini.iniid     = '{$arrPost['iniid']}'" : '';
        $where .= $arrPost['dimid'] ? " AND idi.dimid     =  {$arrPost['dimid']}" : '';
        $innerJoin = '';
        $andOr = "";


        if ($arrPost['iniesfera'] == 'E') {
            $andOr = "OR (iniesfera IN('E','T') AND (iiestuf.estuf = '{$arrPost['estuf']}' OR initodos_estuf  = 'S') )";
        }

        if ($arrPost['iniesfera'] == 'M') {
            $andOr = "OR ( iniesfera IN('M','T') AND (iimun.muncod = '{$arrPost['muncod']}' OR initodos_muncod = 'S') )";
        }

        if ($arrPost['dimid']) {
            $innerJoin = " INNER JOIN par3.iniciativa_indicadores as idi ON idi.iniid = ini.iniid  AND idistatus = 'A'";
        }

        if ($arrPost['iniesfera'] == 'E') {
            $innerJoin .= " LEFT JOIN par3.iniciativa_iniciativas_estados as iiestuf ON iiestuf.iniid = ini.iniid";
        }

        if ($arrPost['iniesfera'] == 'M') {
            $innerJoin .= " LEFT JOIN par3.iniciativa_iniciativas_municipios as iimun   ON iimun.iniid   = ini.iniid";
        }

        $sql = "
            SELECT ini.iniid as codigo, (ini.iniid||' - '||ind.inddsc) as descricao
            FROM {$this->stNomeTabela} ini
            INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
            {$innerJoin}
            {$where}
            AND(iniesfera = 'T'
                {$andOr}
            )
            and ini.iniid in (select iniid from par3.iniciativa_planejamento inp where inp.inuid = ini.iniid and inp.inpstatus = 'A')
            ORDER BY ini.iniid";
//        ob_clean();
//        ver($sql,$arrPost,d);
        $resultado = $this->carregar($sql);
        return $resultado ? simec_preparar_array($resultado) : array();
    }

    public function pegarIniciativa($arrPost)
    {
        $where = $arrPost['iniid'] ? " AND ini.iniid     = '{$arrPost['iniid']}'" : '';
        $where .= $arrPost['iniesfera'] ? " AND iniesfera = '{$arrPost['iniesfera']}'" : '';
        $where .= $arrPost['estuf'] ? " AND iiestuf.estuf = '{$arrPost['estuf']}'" : '';
        $where .= $arrPost['dimid'] ? " AND idi.dimid     =  {$arrPost['dimid']}" : '';
        $innerJoin = '';

        if ($arrPost['dimid']) {
            $innerJoin = " INNER JOIN par3.iniciativa_indicadores as idi ON idi.iniid = ini.iniid ";
        }

        if (!empty($arrPost['iniesfera'])) {
            $innerJoin .= " INNER JOIN par3.iniciativa_iniciativas_estados as iiestuf ON iiestuf.iniid = ini.iniid ";
        }

        if ($arrPost['iniesfera'] == 'M') {
            $where .= $arrPost['muncod'] ? " AND iimun.muncod = '{$arrPost['muncod']}'" : '';
            $innerJoin .= "INNER JOIN par3.iniciativa_iniciativas_municipios as iimun   ON iimun.iniid   = ini.iniid";
        }

        $sql = "
            SELECT *
            FROM {$this->stNomeTabela} ini
            INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
            {$innerJoin}
            WHERE inistatus = 'A' AND inisituacao = 'A'
            {$where}
            ";
        $resultado = $this->carregar($sql);
        return $resultado;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iniid' => array('Digits'),
            'inistatus' => array('allowEmpty' => true),
            'inisituacao' => array(),
            'indid' => array('Digits'),
            'intaid' => array('Digits'),
            'cicid' => array('Digits'),
            'intoid' => array('Digits'),
            'iniesfera' => array('allowEmpty' => true),
            'initodos_estuf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'initodos_muncod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function verificarRestauracao($campos)
    {
        // Pesquisa pelo inistatus = 'I' para verificar se existe uma iniciativa igual mas inativada.
        $campos['inistatus'] = 'I';
        // Define a opção de desconsiderar o campo 'inisituacao' para que este campo não seja verificado na pesquisa. Independerá da situação.
        //$opcoesVerificaExistencia['desconsiderarCampos'] = array("inisituacao");
        return $this->VerificaExistencia($campos);
    }

    // Valida campos existência de iniciativa a partir da aba de Iniciativa Detalhe
    public function validaInclusaoIniciativaDetalhe(array $campos, $opcoes = array())
    {
        // Cria os campos para validação de existência - cve = camposValidacaoExistencia
        $cve = $campos;

        $sql = $this->montarSQLSimples(array('iniid' => $campos['iniid'], 'req' => 'todosCampos'));
        $iniciativa = $this->carregar($sql)[0];
        // Obtem as dimensões que já existe.
        $cve['dimid'] = empty($iniciativa['dimid']) ? array() : explode(', ', $iniciativa['dimid']);
        // Se existir dimensão nos campos, é porque o usuário está criando uma nova dimensão então deve concatenar com o que já existe.
        if (!empty($campos['dimid'])) {
            array_push($cve['dimid'], $campos['dimid']);
        }

        $sqlModalidade = "
						SELECT
							iniid,
							en.nivid,
							ee.etaid,
							mdl.modid,
							inidsd.desid
						FROM par3.iniciativa_modalidades inimdl
						LEFT JOIN par3.ensino_nivel en ON en.nivid = inimdl.nivid
						LEFT JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
						LEFT JOIN par3.modalidade mdl ON mdl.modid = inimdl.modid
						LEFT JOIN par3.iniciativa_desdobramento inidsd ON inidsd.desid = inimdl.desid
						WHERE inimdl.imostatus  = 'A' AND inimdl.iniid = " . $campos['iniid'] . "
				";

        $modalidadesIniciativa = $this->carregar($sqlModalidade);
        $cve['modalidades'] = is_array($modalidadesIniciativa) ? $modalidadesIniciativa : array($modalidadesIniciativa);
        // Se existir dados da modalidade nos campos, é porque o usuário está criando uma nova modalidade então deve concatenar com o que já existe.
        if (!empty($campos['modid'])) {
            array_push($cve['modalidades'], $campos);
        }
        $cve['modalidades'] = array_filter($cve['modalidades']);
        //print_r($cve['modalidades']); exit();
        // Grava os campos da aba de Incluir Iniciativa que são os que serão verificados sobre a existência de outra iniciativa.
        $cve['cicid'] = empty($iniciativa['cicid']) ? null : $iniciativa['cicid'];
        $cve['iniano'] = empty($iniciativa['ano']) ? array() : explode(', ', $iniciativa['ano']);
        $cve['intoid'] = empty($iniciativa['intoid']) ? null : $iniciativa['intoid'];
        $cve['prgid'] = empty($iniciativa['prgid']) ? array() : explode(', ', $iniciativa['prgid']);
        $cve['iarid'] = empty($iniciativa['iarid']) ? array() : explode(', ', $iniciativa['iarid']);
        $cve['proid'] = empty($iniciativa['proid']) ? array() : explode(', ', $iniciativa['proid']);
        $cve['iniesfera'] = empty($iniciativa['iniesfera']) ? null : $iniciativa['iniesfera'];
        $cve['inistatus'] = 'A';
        // Define como opção para não constar a mesma iniciativa, já que é uma edição.
        $opcoesVerificaExistencia['where'] = " iniid != " . $campos['iniid'];
        // Define na opção para não verificar o campo de situação.
        $opcoesVerificaExistencia['desconsiderarCampos'] = array("inisituacao");
        $camposValidaExistencia = $cve;
        $erroExistencia['erros']['validacaoExistencia'] = $this->VerificaExistencia($camposValidaExistencia, $opcoesVerificaExistencia);

        // Caso já exista a iniciativa, retorna como erro.
        foreach ($erroExistencia['erros'] as $key => $value) {
            if (!empty($erroExistencia['erros'][$key])) {
                return $erroExistencia;
            }
        }

        return array('erros' => array());
    }

    // Valida campos obrigatórios e existência de iniciativa - Possui regra de negócio.
    public function validaInclusao(array $campos, $opcoes = array())
    {
        // Cria os campos para validação de existência - cve = camposValidacaoExistencia
        $cve = $campos;

        //Trata opções
        $opcoesVerificaExistencia = array();
        if (array_key_exists('tipo', $opcoes)) {
            if ($opcoes['tipo'] == 'insert') {
                // Cria os campos da aba de Iniciativa Detalhe no array dos campos de validação de existência ($cve) para que conste na query
                // e verifique a não existência (NOT EXISTS).
                $cve['dimid'] = array();
                $cve['nivid'] = array();
                $cve['etaid'] = array();
                $cve['modid'] = array();
                $cve['desid'] = array();
                // Define na opção para não verificar o campo de situação.
                $opcoesVerificaExistencia['desconsiderarCampos'] = array("inisituacao");
                // Se for update, deve verificar se existe apenas se o id for diferente da iniciativa a ser editada.
            } else if ($opcoes['tipo'] == 'update') {
                // Prepara a consulta da iniciativa para obter os valores dos campos da aba de Iniciativa Detalhe
                // para acrescentar no array dos campos de validação de existência ($cve) para que verifica a existência
                $sql = $this->montarSQLSimples(array('iniid' => $campos['iniid'], 'req' => 'todosCampos'));
                $iniciativa = $this->carregar($sql)[0];
                $cve['dimid'] = empty($iniciativa['dimid']) ? array() : explode(', ', $iniciativa['dimid']);
                // Monta query para retornar as linhas da primeira parte da aba de Iniciativa Detalhe - cada linha retorna todas as colunas concatenadas.
                $sqlModalidade = "
						SELECT
							iniid,
							en.nivid,
							ee.etaid,
							mdl.modid,
							inidsd.desid
						FROM par3.iniciativa_modalidades inimdl
						LEFT JOIN par3.ensino_nivel en ON en.nivid = inimdl.nivid
						LEFT JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
						LEFT JOIN par3.modalidade mdl ON mdl.modid = inimdl.modid
						LEFT JOIN par3.iniciativa_desdobramento inidsd ON inidsd.desid = inimdl.desid
						WHERE inimdl.imostatus  = 'A' AND inimdl.iniid = " . $campos['iniid'] . "
				";
                $modalidadesIniciativa = $this->carregar($sqlModalidade);
                $cve['modalidades'] = is_array($modalidadesIniciativa) ? $modalidadesIniciativa : array($modalidadesIniciativa);
                $cve['indid'] = $campos['indid'] ? $campos['indid'] : null;
                // Obtem novamente os valores dos campos de incluir iniciativa para que possa ser utilizado quando editar a aba de Iniciativa Detalhe
                /*$cve['cicid']     = array_key_exists('cicid', $cve)     ?  $cve['cicid']     : (empty($iniciativa['cicid']) ? null : $iniciativa['cicid']);
                $cve['iniano']    = array_key_exists('iniano', $cve)    ?  $cve['iniano']    : (empty($iniciativa['ano']) ? array() : explode(', ', $iniciativa['ano']));
                $cve['intoid']    = array_key_exists('intoid', $cve)    ?  $cve['intoid']    : (empty($iniciativa['intoid']) ? null : $iniciativa['intoid']);
                $cve['prgid']     = array_key_exists('prgid', $cve)     ?  $cve['prgid']     : (empty($iniciativa['prgid']) ? array() : explode(', ', $iniciativa['prgid']));
                $cve['iarid']     = array_key_exists('iarid', $cve)     ?  $cve['iarid']     : (empty($iniciativa['iarid']) ? array() : explode(', ', $iniciativa['iarid']));
                $cve['proid']     = array_key_exists('proid', $cve)     ?  $cve['proid']     : (empty($iniciativa['proid']) ? array() : explode(', ', $iniciativa['proid']));
                $cve['iniesfera'] = array_key_exists('iniesfera', $cve) ?  $cve['iniesfera'] : (empty($iniciativa['iniesfera']) ? null : $iniciativa['iniesfera']);*/
                //print_r($cve); exit();
                // Define na opção para não verificar o campo de situação.
                $opcoesVerificaExistencia['desconsiderarCampos'] = array("inisituacao");
                // Define parte do WHERE da verificação de existência para que não conste a própria iniciativa em edição.
                $opcoesVerificaExistencia['where'] = " iniid != " . $campos['iniid'];
            }
        }

        $retorno = false;
        //Valida campos.
        $opcoesValidarInput = array(
            'desconsiderarCampos' => array()
        );
        //print_r($opcoes['desconsiderarCamposPreenchimento']); exit();
        if (array_key_exists('desconsiderarCamposPreenchimento', $opcoes)) {
            $opcoesValidarInput['desconsiderarCampos'] = $opcoes['desconsiderarCamposPreenchimento'];
        }

        $erroInput = $this->validarInput($campos, $opcoesValidarInput);

        // Caso haja tenha algum campo obrigatório não preenchido, retorna como erro.
        foreach ($erroInput['erros'] as $key => $value) {
            if (!empty($erroInput['erros'][$key])) {
                return $erroInput;
            }
        }

        // Valida existência.
        // Remove campos que não serão verificados
        unset($cve['intaid']);
        unset($cve['estuf']);
        unset($cve['initodos_estuf']);
        unset($cve['muncod']);
        unset($cve['initodos_muncod']);
        unset($cve['gtmid']);
        // Insere campos da aba de Iniciativa Detalhe
        //$cve['dimid']
        $camposValidaExistencia = $cve;
        $erroExistencia['erros']['validacaoExistencia'] = $this->VerificaExistencia($camposValidaExistencia, $opcoesVerificaExistencia);
        if (!empty($erroExistencia['erros']['validacaoExistencia'])) {
            $erroExistencia['erros']['validacaoExistencia'] = 'Iniciativa informada já existe!';
        }

        // Caso já exista a iniciativa, retorna como erro.
        foreach ($erroExistencia['erros'] as $key => $value) {
            if (!empty($erroExistencia['erros'][$key])) {
                return $erroExistencia;
            }
        }

        // Como não encontrou erro, junta as verificações no array de erros e o retorna.
        $erros['erros'] = array_merge($erroInput['erros'], $erroExistencia['erros']);

        return $erros;
    }

    public function validarInput(array $campos, $opcoes = array())
    {
        //print_r($opcoes); exit();
        //Campos
        $erros['erros']['inisituacao'] = array();
        $erros['erros']['indid'] = array();
        $erros['erros']['intaid'] = array();
        $erros['erros']['cicid'] = array();
        $erros['erros']['intoid'] = array();
        $erros['erros']['iniesfera'] = array();
        // Tabelas externas
        $erros['erros']['iarid'] = array();
        $erros['erros']['proid'] = array();
        $erros['erros']['prgid'] = array();
        $erros['erros']['estuf'] = array();
        $erros['erros']['gtmid'] = array();
        $erros['erros']['iniano'] = array();
        $erros['erros']['muncod'] = array();

        //VALIDA CAMPOS
        if ($campos['inisituacao'] == '' || $campos['inisituacao'] == null) {
            array_push($erros['erros']['inisituacao'], 'O campo não pode ser vazio');
        }
        if ($campos['indid'] == '' || $campos['indid'] == null) {
            array_push($erros['erros']['indid'], 'O campo não pode ser vazio');
        }
        if ($campos['intaid'] == '' || $campos['intaid'] == null) {
            array_push($erros['erros']['intaid'], 'O campo não pode ser vazio');
        }
        if ($campos['cicid'] == '' || $campos['cicid'] == null) {
            array_push($erros['erros']['cicid'], 'O campo não pode ser vazio');
        }
        if ($campos['intoid'] == '' || $campos['intoid'] == null) {
            array_push($erros['erros']['intoid'], 'O campo não pode ser vazio');
        }
        if ($campos['iniesfera'] == '' || $campos['iniesfera'] == null) {
            array_push($erros['erros']['iniesfera'], 'O campo não pode ser vazio');
        }
        if ($campos['iarid'] == '' || $campos['iarid'] == null) {
            array_push($erros['erros']['iarid'], 'O campo não pode ser vazio');
        }
        if (($campos['estuf'] == '' || $campos['estuf'] == null) && array_search('estuf', $opcoes['desconsiderarCampos']) === false) {
            array_push($erros['erros']['estuf'], 'O campo não pode ser vazio');
        }
        if ($campos['iniesfera'] == "T" || $campos['iniesfera'] == "M" || $campos['iniesfera'] == '' || $campos['iniesfera'] == null) {
            if (($campos['muncod'] == '' || $campos['muncod'] == null) && array_search('muncod', $opcoes['desconsiderarCampos']) === false) {
                array_push($erros['erros']['muncod'], 'O campo não pode ser vazio');
            }
            // Comentado pois campo não foi mais considerado obrigatório:
            /*if ($campos['gtmid'] == '' || $campos['gtmid'] == null) {
                array_push($erros['erros']['gtmid'], 'O campo não pode ser vazio');
            }*/
        }
        if ($campos['prgid'] == '' || $campos['prgid'] == null) {
            array_push($erros['erros']['prgid'], 'O campo não pode ser vazio');
        }
        if ($campos['iniano'] == '' || $campos['iniano'] == null) {
            array_push($erros['erros']['iniano'], 'O campo não pode ser vazio');
        }

        return $erros;
    }

    public function verificaExistencia($campos, $opcoes = array())
    {
        //print_r($campos); exit();
        $retorno = false;
        $where = "";
        $whereInisituacao = $campos['inisituacao'] ? " inisituacao IN('{$campos['inisituacao']}') " : " inisituacao IS NULL";

        if ($opcoes) {
            if (array_key_exists('desconsiderarCampos', $opcoes)) {
                $whereInisituacao = in_array('inisituacao', $opcoes['desconsiderarCampos']) ? '' : $whereInisituacao;
            }
            if (array_key_exists('where', $opcoes)) {
                $where .= $opcoes['where'];
            }
        }

        $where .= !empty($where) && !empty($whereInisituacao) ? " AND $whereInisituacao" : "";
        if (array_key_exists('indid', $campos)) {
            $where .= $where ? " AND " : $where;
            $where .= " indid";
            $where .= $campos['indid'] ? " IN({$campos['indid']}) " : " IS NULL ";
        }
        $where .= $where ? " AND " : $where;
        $where .= " cicid";
        $where .= $campos['cicid'] ? " IN({$campos['cicid']}) " : " IS NULL ";
        if (array_key_exists('intaid', $campos)) {
            $where .= $where ? " AND " : $where;
            $where .= " intaid";
            $where .= $campos['intaid'] ? " IN({$campos['intaid']}) " : " IS NULL ";
        }
        $where .= $where ? " AND " : $where;
        $where .= " intoid";
        $where .= $campos['intoid'] ? " IN({$campos['intoid']}) " : " IS NULL ";
        $where .= $where ? " AND " : $where;
        $where .= " iniesfera";
        $where .= $campos['iniesfera'] ? " IN('{$campos['iniesfera']}') " : " IS NULL ";
        $where .= $where ? " AND " : $where;
        $where .= " inistatus";
        $where .= $campos['inistatus'] ? " = '{$campos['inistatus']}' " : " A ";
        if (array_key_exists('initodos_estuf', $campos)) {
            $where .= $where ? " AND " : $where;
            $where .= " initodos_estuf";
            $where .= $campos['initodos_estuf'] ? " = '{$campos['initodos_estuf']}' " : " IS NULL ";
        }
        if (array_key_exists('initodos_muncod', $campos)) {
            $where .= $where ? " AND " : $where;
            $where .= " initodos_muncod";
            $where .= $campos['initodos_muncod'] ? " = '{$campos['initodos_muncod']}' " : " IS NULL ";
        }
        // WHERE para campos de tabelas externas N x N.
        //$arrIarid = explode(',', $campos['iarid']);
        foreach ($campos['iarid'] as $iniciativaArea) {
            $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_areas WHERE iniid = ini.iniid AND iarid = $iniciativaArea AND iiarstatus = 'A')";
        }
        // Verifica se tem a mesma quantidade
        $where .= " AND (SELECT COUNT(iarid) FROM par3.iniciativa_iniciativas_areas WHERE iniid = ini.iniid AND iiarstatus = 'A' GROUP BY iniid)" . (count($campos['iarid']) == 0 ? "IS NULL" : " = " . count($campos['iarid']));
        //$arrProid = explode(',', $campos['proid']);
        foreach ($campos['proid'] as $projeto) {
            $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_projetos WHERE iniid = ini.iniid AND proid = $projeto AND iiprostatus = 'A' )";
        }
        // Verifica se tem a mesma quantidade
        $where .= " AND (SELECT COUNT(proid) FROM par3.iniciativa_iniciativas_projetos WHERE iniid = ini.iniid  AND iiprostatus = 'A' GROUP BY iniid)" . (count($campos['proid']) == 0 ? "IS NULL" : " = " . count($campos['proid']));

        if (empty($campos['prgid'])) {
            $where .= " AND NOT EXISTS(SELECT * FROM par3.iniciativa_iniciativas_programas WHERE iniid = ini.iniid AND iiprgstatus = 'A' )";
        } else {
            foreach ($campos['prgid'] as $programa) {
                $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_programas WHERE iniid = ini.iniid AND prgid = $programa AND iiprgstatus = 'A' )";
            }
            // Verifica se tem a mesma quantidade
            $where .= " AND (SELECT COUNT(prgid) FROM par3.iniciativa_iniciativas_programas WHERE iniid = ini.iniid  AND iiprgstatus = 'A' GROUP BY iniid)" . (count($campos['prgid']) == 0 ? "IS NULL" : " = " . count($campos['prgid']));
        }

        if (array_key_exists('estuf', $campos)) {
            foreach ($campos['estuf'] as $estado) {
                $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_estados WHERE iniid = ini.iniid AND estuf = '$estado' AND iiestufstatus = 'A' )";
            }
            // Verifica se tem a mesma quantidade
            $where .= " AND (SELECT COUNT(estuf) FROM par3.iniciativa_iniciativas_estados WHERE iniid = ini.iniid  AND iiestufstatus = 'A' GROUP BY iniid)" . (count($campos['estuf']) == 0 ? "IS NULL" : " = " . count($campos['estuf']));
        }

        if (array_key_exists('gtmid', $campos)) {
            if (empty(array_filter($campos['gtmid']))) {
                $where .= " AND NOT EXISTS(SELECT * FROM par3.iniciativa_iniciativas_grupo_tipo_municipio WHERE iniid = ini.iniid AND iigtmstatus = 'A' )";
            } else {
                foreach ($campos['gtmid'] as $grupoTipoMunicipio) {
                    $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_grupo_tipo_municipio WHERE iniid = ini.iniid AND gtmid = $grupoTipoMunicipio AND iigtmstatus = 'A' )";
                }
                // Verifica se tem a mesma quantidade
                $where .= " AND (SELECT COUNT(gtmid) FROM par3.iniciativa_iniciativas_grupo_tipo_municipio WHERE iniid = ini.iniid  AND iigtmstatus = 'A' GROUP BY iniid)" . (count($campos['gtmid']) == 0 ? "IS NULL" : " = " . count($campos['gtmid']));
            }
        }
        //$arrIniano = explode(',', $campos['iniano']);
        foreach ($campos['iniano'] as $iniano) {
            $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_anos WHERE iniid = ini.iniid AND iniano = $iniano AND iianostatus = 'A' )";
        }
        // Verifica se tem a mesma quantidade
        $where .= " AND (SELECT COUNT(iniid) FROM par3.iniciativa_iniciativas_anos WHERE iniid = ini.iniid  AND iianostatus = 'A' GROUP BY iniid) " . (count($campos['iniano']) == 0 ? "IS NULL" : " = " . count($campos['iniano']));
        // Campo Municípios
        if (array_key_exists('muncod', $campos)) {
            if (empty(array_filter($campos['muncod']))) {
                $where .= " AND NOT EXISTS(SELECT * FROM par3.iniciativa_iniciativas_municipios WHERE iniid = ini.iniid AND iimunidstatus = 'A' )";
            } else {
                foreach ($campos['muncod'] as $municipio) {
                    $where .= " AND EXISTS(SELECT * FROM par3.iniciativa_iniciativas_municipios WHERE iniid = ini.iniid AND muncod = $municipio AND iimunidstatus = 'A' )";
                }
            }
            // Verifica se tem a mesma quantidade
            $where .= " AND (SELECT COUNT(muncod) FROM par3.iniciativa_iniciativas_municipios WHERE iniid = ini.iniid  AND iimunidstatus = 'A' GROUP BY iniid) " . (count($campos['muncod']) == 0 ? "IS NULL" : " = " . count($campos['muncod']));
        }
        // Campo Dimensao
        if (array_key_exists('dimid', $campos)) {
            if (empty(array_filter($campos['dimid']))) {
                $where .= " AND NOT EXISTS(
								SELECT *
								FROM par3.iniciativa_indicadores iniind
								INNER JOIN par3.dimensao dms ON dms.dimid = iniind.dimid
								WHERE iniind.iniid = ini.iniid AND iniind.idistatus = 'A'
							)";
            } else {
                foreach ($campos['dimid'] as $dimensao) {
                    $where .= " AND EXISTS(
								SELECT *
								FROM par3.iniciativa_indicadores iniind
								INNER JOIN par3.dimensao dms ON dms.dimid = iniind.dimid
								WHERE iniind.iniid = ini.iniid AND iniind.idistatus = 'A' AND dms.dimid = $dimensao
							)";
                }
            }
            // Verifica se tem a mesma quantidade
            $where .= " AND (
						SELECT COUNT(dms.dimid)
						FROM par3.iniciativa_indicadores iniind
						INNER JOIN par3.dimensao dms ON dms.dimid = iniind.dimid
						WHERE iniind.iniid = ini.iniid AND iniind.idistatus = 'A'
						GROUP BY iniid)
					  " . (count($campos['dimid']) == 0 ? "IS NULL" : " = " . count($campos['dimid']));
        }
        // Campos da parte de Modalidade da aba de Iniciativa Descrição - Verifica se todas as linhas da tabela de modalidades batem
        if (array_key_exists('modalidades', $campos)) {
            $campos['modalidades'] = array_filter($campos['modalidades']);
            if (empty(array_filter($campos['modalidades']))) {
                $where .= " AND NOT EXISTS(
								SELECT *
								FROM par3.iniciativa_modalidades inimdl
								INNER JOIN par3.modalidade mdl ON mdl.modid = inimdl.modid
								WHERE inimdl.iniid = ini.iniid AND inimdl.imostatus  = 'A'
							)";
            } else {
                foreach ($campos['modalidades'] as $modalidade) {
                    $where .= " AND EXISTS(
						SELECT *
						FROM par3.iniciativa_modalidades inimdl
						INNER JOIN par3.ensino_nivel en ON en.nivid = inimdl.nivid
						INNER JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
						INNER JOIN par3.modalidade mdl ON mdl.modid = inimdl.modid
						INNER JOIN par3.iniciativa_desdobramento inidsd ON inidsd.desid = inimdl.desid
						WHERE
							inimdl.iniid = ini.iniid
							AND inimdl.imostatus  = 'A'
							AND en.nivid = " . $modalidade['nivid'] . "
							AND ee.etaid = " . $modalidade['etaid'] . "
							AND mdl.modid = " . $modalidade['modid'] . "
							AND inidsd.desid = " . $modalidade['desid'] . "
					)";
                }
            }
            // Verifica se tem a mesma quantidade
            $where .= " AND (
							SELECT COUNT(inimdl.imoid)
							FROM par3.iniciativa_modalidades inimdl
							WHERE inimdl.iniid = ini.iniid AND inimdl.imostatus  = 'A'
							GROUP BY iniid)
					  " . (count($campos['modalidades']) == 0 ? "IS NULL" : " = " . count($campos['modalidades']));
        }

        $where = " WHERE $where";

        $sql = "SELECT iniid
				FROM  {$this->stNomeTabela} as ini
				$where";;
// 		ver($sql); exit();
        $retorno = $this->carregar($sql);
        $retorno = $retorno ? $retorno : array();

        return $retorno;
    }

    public function montarSQLSimples($arrPost)
    {
        $iniid = is_array($arrPost['iniid']) ? implode($arrPost['iniid']) : ($arrPost['iniid'] ? $arrPost['iniid'] : '');
        $inddsc = sanitizar_string_pesquisa($arrPost['inddsc']);
        $intoid = $arrPost['intoid'] ? (is_array($arrPost['intoid']) ? implode($arrPost['intoid']) : $arrPost['intoid']) : '';
        $dimid = $arrPost['dimid'] ? (is_array($arrPost['dimid']) ? implode($arrPost['dimid']) : $arrPost['dimid']) : '';
        $intaid = $arrPost['intaid'] ? (is_array($arrPost['intaid']) ? implode($arrPost['intaid']) : $arrPost['intaid']) : '';
        $cicid = $arrPost['cicid'] ? (is_array($arrPost['cicid']) ? implode($arrPost['cicid']) : $arrPost['cicid']) : '';
        $iarid = $arrPost['iarid'] ? (is_array($arrPost['iarid']) ? implode($arrPost['iarid']) : $arrPost['iarid']) : '';
        $prgid = $arrPost['prgid'] ? (is_array($arrPost['prgid']) ? implode($arrPost['prgid']) : $arrPost['prgid']) : '';
        $inisituacao = $arrPost['inisituacao'] ? $arrPost['inisituacao'] : '';

        $where = $arrPost['inddsc'] ? " AND upper(removeacento(inid.inddsc)) LIKE '%{$inddsc}%'" : '';
        $where .= $arrPost['iniid'] ? " AND ini.iniid IN ({$iniid})" : '';
        $where .= $arrPost['intoid'] ? " AND ini.intoid IN ({$intoid})" : '';
        $where .= $arrPost['intaid'] ? " AND ini.intaid IN ({$intaid})" : '';
        $where .= $arrPost['cicid'] ? " AND ini.cicid IN ({$cicid})" : '';
        $where .= $arrPost['iarid'] ? " AND iniiar.iarid IN ({$iarid})" : '';
        $where .= $arrPost['prgid'] ? " AND iniiprg.prgid IN ({$prgid})" : '';
        $prgdsc = sanitizar_string_pesquisa($arrPost['prgdsc']);
        $where .= $arrPost['prgdsc']?" AND (upper(removeacento(prgdsc)) LIKE upper(removeacento('%{$prgdsc}%')) OR upper(prgabreviatura) LIKE upper(removeacento('%{$prgdsc}%')))":'';
        $where .= $arrPost['inisituacao'] ? " AND ini.inisituacao = '{$inisituacao}'" : '';


        if($arrPost['iniobraproinfancia'] == 'true'){
            $where .= " AND ini.iniobraproinfancia = 't'";
        }else if ($arrPost['iniobraproinfancia'] == 'false'){
            $where .= " AND (ini.iniobraproinfancia = 'f' OR ini.iniobraproinfancia IS NULL )";
        }

        // Se existir algum filtro relacionados com os indicadores, cria o WHERE dentro do LEFT JOIN do iniind:
        $whereIndicadores = $dimid ? " AND iniind.dimid IN ({$dimid}) " : '';
        //echo 'dimid: '; print_r($dimid); echo "<br><br>";
        // Se montou algum where dos indicadores, insere necessidade de existência dos indicadores (iniind) mudando o JOIN para INNER da iniciativa (ini):
        $joinIndicadores = $whereIndicadores ? "INNER JOIN" : "LEFT JOIN";

        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}" : "";

        if ($arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' || $arrPost['req'] == 'todosCampos') {
            $contador = "ROW_NUMBER() OVER ({$orderBy}) AS qtd,";
            $inicio = "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (";
            $select = "
        		SELECT DISTINCT
					ini.iniid as id,
					ini.iniid as codigo,
					inid.inddsc as descricao,
					inita.intadsc as tipo_atendimento,
        			cp.cicid as cicid,
					cp.cicdsc as ciclo,
					string_agg(distinct iniian.iniano::text, ', ') as ano,
        			inito.intoid as intoid,
					inito.intodsc as tipo_objeto,
        			string_agg(distinct iniar.iarid::text, ', ') as iarid,
					string_agg(distinct iniar.iardsc, ', ') as area_relacionada,
        			ini.iniesfera as iniesfera,
					CASE (ini.iniesfera) WHEN 'M' THEN 'Municipal' WHEN 'E' THEN 'Estadual' ELSE 'Todos' END as esfera,
					string_agg(distinct prj.proid::text, ', ') as proid,
        			string_agg(distinct prj.prodsc, ', ') as projeto,
        			string_agg(distinct prg.prgid::text, ', ') as prgid,
					string_agg(distinct prg.prgdsc, ', ') as programa,
        			CASE(ini.initodos_estuf) WHEN 'S' THEN 'Todos' WHEN 'N' THEN string_agg(distinct est.estuf, ', ') END as estado,
					CASE(ini.initodos_muncod) WHEN 'S' THEN 'Todos' WHEN 'N' THEN string_agg(distinct mun.mundescricao, ', ') END as municipio,
					string_agg(distinct gtm.gtmdsc, ', ') as grupo_municipio,
					inimdl.nivel as nivel,
					inimdl.etapa as etapa,
					inimdl.modalidade as modalidade,
					inimdl.desdobramento as desdobramento,
        			iniind.dimid as dimid,
					iniind.dmddsc as dimensao,
					iniind.area as area,
					iniind.indicador as indicador,
					iniind.componente as componente,
					iniicg.item_composicao as item_composicao,
					iniicg.item_composicao_grupo as item_composicao_grupo
        	";
            $fim = ") as foo";
        } else {
            $contador = '';
            $inicio = '';
            $select = "
        		SELECT DISTINCT
					ini.iniid as id,
					ini.iniid as codigo,
					inid.inddsc as descricao,
					inito.intodsc as tipo_objeto,
					inita.intadsc as tipo_atendimento,
					iniind.dimcod as dimensao,
					cp.cicdsc as ciclo,
					string_agg(distinct iniar.iardsc, ', ') as area_relacionada,
					prgabreviatura as sigla_programa
        	";
            $fim = '';
        }

        $select .= ", CASE WHEN (ini.inisituacao = 'A') THEN 'Ativo' ELSE 'Inativo' END as situacao";

        $sql = "
        	{$inicio}
	        {$select}
			FROM
				par3.iniciativa as ini
			LEFT JOIN par3.iniciativa_descricao as inid ON inid.indid = ini.indid
			LEFT JOIN par3.iniciativa_tipos_objeto as inito ON inito.intoid = ini.intoid
			LEFT JOIN par3.iniciativa_tipos_atendimento as inita ON inita.intaid = ini.intaid
			LEFT JOIN(
				SELECT
					iniid,
					string_agg(distinct en.nivdsc, ', ') as nivel,
					string_agg(distinct ee.etadsc, ', ') as etapa,
					string_agg(distinct mdl.moddsc, ', ') as modalidade,
					string_agg(distinct inidsd.desdsc, ', ') as desdobramento
				FROM par3.iniciativa_modalidades inimdl
				LEFT JOIN par3.ensino_nivel en ON en.nivid = inimdl.nivid
				LEFT JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
				LEFT JOIN par3.modalidade mdl ON mdl.modid = inimdl.modid
				LEFT JOIN par3.iniciativa_desdobramento inidsd ON inidsd.desid = inimdl.desid
				WHERE inimdl.imostatus  = 'A'
				GROUP BY inimdl.iniid
			) inimdl ON inimdl.iniid = ini.iniid
			{$joinIndicadores}(
				SELECT
					iniind.iniid,
					string_agg(distinct dms.dimid::text, ', ') as dimid,
					string_agg(distinct dms.dimdsc, ', ') as dmddsc,
					string_agg(distinct are.aredsc, ', ') as area,
					string_agg(distinct ind.inddsc, ', ') as indicador,
					string_agg(distinct crt.crtdsc, ', ') as componente,
					string_agg(distinct dms.dimcod::text, ', ') as dimcod
				FROM par3.iniciativa_indicadores iniind
				LEFT JOIN par3.dimensao dms ON dms.dimid = iniind.dimid
				LEFT JOIN par3.area are ON are.areid = iniind.areid
				LEFT JOIN par3.indicador ind ON ind.indid = iniind.indid
				LEFT JOIN par3.criterio crt ON crt.crtid = iniind.crtid
				WHERE iniind.idistatus = 'A' {$whereIndicadores}
				GROUP BY iniind.iniid
			) iniind ON iniind.iniid = ini.iniid
			LEFT JOIN(
				SELECT
					iniicg.iniid,
					string_agg(distinct itc.itcdsc, ', ') as item_composicao,
					string_agg(distinct itcg.igrnome, ', ') as item_composicao_grupo
				FROM par3.iniciativa_itenscomposicao_grupo iniicg
				LEFT JOIN par3.itenscomposicao itc ON itc.itcid = iniicg.itcid
				LEFT JOIN par3.itenscomposicao_grupos itcg ON itcg.igrid = iniicg.igrid
				GROUP BY iniicg.iniid
			) iniicg ON iniicg.iniid = ini.iniid
			LEFT JOIN par3.ciclo_par cp ON cp.cicid = ini.cicid
			LEFT JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iniid = ini.iniid AND iniiar.iiarstatus = 'A'
			LEFT JOIN par3.iniciativa_areas as iniar ON iniar.iarid = iniiar.iarid
			LEFT JOIN par3.iniciativa_iniciativas_anos iniian ON iniian.iniid = ini.iniid AND iniian.iianostatus = 'A'
			LEFT JOIN par3.iniciativa_iniciativas_projetos iniiprj ON iniiprj.iniid = ini.iniid AND iniiprj.iiprostatus = 'A'
			LEFT JOIN par3.projeto prj ON prj.proid = iniiprj.proid
			LEFT JOIN par3.iniciativa_iniciativas_programas iniiprg ON iniiprg.iniid = ini.iniid AND iniiprg.iiprgstatus = 'A'
			LEFT JOIN par3.programa prg ON prg.prgid = iniiprg.prgid
			LEFT JOIN par3.iniciativa_iniciativas_estados iniiestuf ON iniiestuf.iniid = ini.iniid AND iiestufstatus = 'A'
			LEFT JOIN territorios.estado est ON est.estuf = iniiestuf.estuf
			LEFT JOIN par3.iniciativa_iniciativas_municipios iniimun ON iniimun.iniid = ini.iniid AND iimunidstatus = 'A'
			LEFT JOIN territorios.municipio mun ON mun.muncod = iniimun.muncod
			LEFT JOIN par3.iniciativa_iniciativas_grupo_tipo_municipio iniigtm ON iniigtm.iniid = ini.iniid AND iigtmstatus = 'A'
			LEFT JOIN territorios.grupotipomunicipio gtm ON gtm.gtmid = iniigtm.gtmid
			WHERE
				ini.inistatus = 'A'
				{$where}
			GROUP BY
				ini.iniid,
				inid.inddsc,
				inito.intoid,
				inito.intodsc,
				inita.intadsc,
				iniind.dimid,
				iniind.dmddsc,
				cp.cicid,
				cp.cicdsc,
				ini.inisituacao,
				ini.iniesfera,
				inimdl.nivel,
				inimdl.etapa,
				inimdl.modalidade,
				inimdl.desdobramento,
				iniind.area,
				iniind.indicador,
				iniind.componente,
				iniicg.item_composicao_grupo,
				iniicg.item_composicao,
				prgabreviatura,
				dimcod
	        {$orderBy}
        	{$fim}
        ";

        return $sql;
    }

    // CÓDIGO LEGADO
    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    //protected $stNomeTabela = "par3.subacao";

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    /*protected $arChavePrimaria = array (
            "sbaid"
    );*/

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    /*protected $arAtributos = array (
            "sbaid" => null,
            "sbadsc" => null,
            "sbaordem" => null,
            "sbaobra" => null,
            "sbaestrategiaimplementacao" => null,
            "sbaptres" => null,
            "sbanaturezadespesa" => null,
            "sbamonitoratecnico" => null,
            "docid" => null,
            "frmid" => null,
            "indid" => null,
            "foaid" => null,
            "undid" => null,
            "ppsid" => null,
            "aciid" => null,
            "prgid" => null,
            "ptsid" => null,
            "sbacronograma" => null,
            "sbappspeso" => null,
            "sbaobjetivo" => null,
            "sbatexto" => null,
            "sbacobertura" => null,
            "usucpf" => null,
            "sbadataalteracao" => null,
            "sbastatus" => null,
            "sbaextraordinaria" => null,
            "sbaemenda" => null,
            "sbaidpai" => null,
            "sbareformulacao" => null,
            "sbadatareformulacao" => null,
            "sbaextraordinariatramita" => null,
            "sbapropostaemenda" => null,
            "sbanovopar" => null,
            "inuid_par3" => null
    );
    protected $stOrdem = null;*/
    public function recuperarProgramas($dados)
    {
        $modelUnidade = new Par3_Model_InstrumentoUnidade ($dados ['inuid']);
        $itrid = $modelUnidade->itrid == '2' ? '2' : '1';

        $sql = "SELECT DISTINCT
                    prg.prgid as codigo,
                	prg.prgdsc as programa,
                    SUM(ico.icovalor*ico.icoquantidade) as valor_iniciativas
                FROM par.programa prg
		        INNER JOIN par.propostasubacao      pps ON pps.prgid = prg.prgid
                LEFT  JOIN par.subacao 		        sba
                	LEFT JOIN par.subacaodetalhe 	     sbd ON sbd.sbaid = sba.sbaid
                	LEFT JOIN par.subacaoitenscomposicao ico ON ico.sbaid = sbd.sbaid AND ico.icoano = sbd.sbdano
                	ON sba.ppsid = pps.ppsid AND sba.sbastatus = 'A' AND sba.inuid_par3 IS NOT NULL
                INNER JOIN par.indicador            ind ON ind.indid = pps.indid
                INNER JOIN par.area                 are ON are.areid = ind.areid
                INNER JOIN par.dimensao             dim ON dim.dimid = are.dimid AND dim.itrid = $itrid
            	INNER JOIN par.propostatiposubacao  pts ON pts.ptsid = pps.ptsid AND pts.ptsstatus = 'A'
		        WHERE
                    prg.prgstatus = 'A'
		            --AND prg.prgid NOT IN (2)
		            AND pps.ppsnovopar = 'S'
		            --AND dim.itrid IN (1,3)
		        GROUP BY
                    prg.prgid,
                	prg.prgdsc
                ORDER BY
                	programa";

        $arrProgramas = $this->carregar($sql);

        return is_array($arrProgramas) ? $arrProgramas : array();
    }

    public function recuperarIniciativasPrograma($dados)
    {
        $modelUnidade = new Par3_Model_InstrumentoUnidade ($dados ['inuid']);
        $itrid = $modelUnidade->itrid == '2' ? '2' : '1';

        $where = array(
            "AND pps.ppsnovopar = 'S'"
        );

        foreach ($dados as $k => $dado) {
            switch ($k) {
                case "prgid" :
                    if (is_array($dado)) {
                        $where [] = "pps.prgid IN (" . implode(', ', $dado) . ")";
                    } else {
                        $where [] = "pps.prgid = $dado";
                    }
                    break;
                case "ppsid" :
                    if (is_array($dado)) {
                        $where [] = "pps.ppsid IN (" . implode(', ', $dado) . ")";
                    } else {
                        $where [] = "pps.ppsid = $dado";
                    }
                    break;
                case "sbaid" :
                    $whereSbaid = "AND sba.sbaid = $dado";
                    break;
                case "inuid" :
                    $whereSbaid = "AND sba.inuid_par3 = $dado";
                    break;
            }
        }

        $sql = "SELECT DISTINCT
                    sba.sbaid,
                    pps.ppsid,
                	par.retornacodigopropostasubacao(pps.ppsid)||' '||pps.ppsdsc as descricao,
                	CASE
                		WHEN pps.ppscronograma = 1 THEN 'Global'
                		WHEN pps.ppscronograma = 2 THEN 'Escola'
                		ELSE 'N/A'
                	END as cronograma,
                    pts.ptsdescricao,
                    tor.tordescricao,
                    SUM(
                	CASE
                		WHEN pps.ppscronograma = 1 THEN ico.icovalor*ico.icoquantidade
                		WHEN pps.ppscronograma = 2 THEN ico.icovalor*sei.seiqtd
                	END) as valor_iniciativa
                FROM par.propostasubacao 	pps
                INNER JOIN par.indicador               ind ON ind.indid = pps.indid
                INNER JOIN par.area                    area ON area.areid = ind.areid
                INNER JOIN par.dimensao                dim ON dim.dimid = area.dimid AND dim.itrid = $itrid

            	INNER JOIN par.propostatiposubacao     pts ON pts.ptsid = pps.ptsid AND pts.ptsstatus = 'A'
            	INNER JOIN par3.tipoorcamento          tor ON tor.torid = pps.torid AND tor.torstatus = 'A'
            	LEFT  JOIN par.propostasubacaoestados  pse ON pse.ppsid = pps.ppsid
                LEFT  JOIN par3.instrumentounidade     inu ON inu.estuf = pse.estuf --AND inu.itrid IN (1,3)
                LEFT  JOIN par.subacao 		sba
                	LEFT JOIN par.subacaodetalhe 	               sbd ON sbd.sbaid = sba.sbaid
                	LEFT JOIN par.subacaoitenscomposicao           ico ON ico.sbaid = sbd.sbaid AND ico.icoano = sbd.sbdano AND ico.icostatus = 'A'
                	LEFT JOIN par.subacaoescolas                   ses ON ses.sbaid = sba.sbaid AND ses.sesano = sbd.sbdano AND ses.sesstatus = 'A'
                	LEFT JOIN par.subescolas_subitenscomposicao    sei ON sei.sesid = ses.sesid AND sei.icoid = ico.icoid
                	ON sba.ppsid = pps.ppsid AND sba.sbastatus = 'A' $whereSbaid
                WHERE
                    (
                    CASE WHEN pse.ppsid IS NOT NULL
                        THEN inu.inuid = {$dados['inuid']}
		                ELSE 1 = 1
		            END
	                )
                	" . implode(' AND ', $where) . "
                GROUP BY
                    sba.sbaid,
                    pps.ppsid,
                	pps.ppsdsc,
                    tor.tordescricao,
                	pts.ptsdescricao
                ORDER BY
                	descricao";
        // ver($sql);
        $arrIniciativas = $this->carregar($sql);

        return is_array($arrIniciativas) ? $arrIniciativas : array();
    }

    public function recuperarIniciativa($dados)
    {
        $where = array(
            '1=1'
        );
        $whereIniciativa = array(
            ' AND 1=1'
        );

        foreach ($dados as $k => $dado) {
            switch ($k) {
                case "ppsid" :
                    if (is_array($dado)) {
                        $where [] = "pps.ppsid IN (" . implode(', ', $dado) . ")";
                    } else {
                        $where [] = "pps.ppsid = $dado";
                    }
                    break;
                case "inuid" :
                    $whereIniciativa [] = "sba.inuid_par3 = $dado";
                    $whereInuid = "iuo.inuid = $dado";
                    break;
                case "sbaid" :
                    if ($dado > 0) {
                        $whereIniciativa [] = "sba.sbaid = $dado";
                    }
                    break;
            }
        }

        $sql = "SELECT DISTINCT
		            sbd.sbdid,
                    sba.sbaid,
                    sba.inuid_par3 as inuid,
                    pps.ppsid,
                    pps.prgid,
                	par.retornacodigopropostasubacao(pps.ppsid)||' '||pps.ppsdsc as ppsdsc,
                	CASE
                		WHEN pps.ppscronograma = 1 THEN 'Global'
                		WHEN pps.ppscronograma = 2 THEN 'Escola'
                		ELSE 'N/A'
                	END as cronograma,
                    pps.ppscronograma,
                    pts.ptsdescricao,
                    tor.torid,
		            tor.tordescricao,
		            iuo.iuovalor,
		            sbd.sbdplanointerno,
		            sbd.sbdptres,
                    count(DISTINCT escid) as qtd_escolas,
                	SUM(
                	CASE
                		WHEN pps.ppscronograma = 1 THEN ico.icovalor*ico.icoquantidade
                		WHEN pps.ppscronograma = 2 THEN ico.icovalor*sei.seiqtd
                	END) as valor_iniciativa
                FROM par.propostasubacao 	pps
            	INNER JOIN par.propostatiposubacao pts ON pts.ptsid = pps.ptsid AND pts.ptsstatus = 'A'
            	LEFT  JOIN par3.tipoorcamento      tor ON tor.torid = pps.torid AND tor.torstatus = 'A'
		        LEFT  JOIN par3.instrumentounidade_orcamento iuo ON iuo.torid = tor.torid AND $whereInuid
                LEFT JOIN par.subacao 		sba
                	LEFT JOIN par.subacaodetalhe                   sbd ON sbd.sbaid = sba.sbaid
		            LEFT JOIN par.subacaoitenscomposicao           ico ON ico.sbaid = sba.sbaid AND ico.icoano = sbd.sbdano AND ico.icostatus = 'A'
                	LEFT JOIN par.subacaoescolas                   ses ON ses.sbaid = sba.sbaid AND ses.sesano = sbd.sbdano AND ses.sesstatus = 'A'
                	LEFT JOIN par.subescolas_subitenscomposicao    sei ON sei.sesid = ses.sesid AND sei.icoid = ico.icoid
                	ON sba.ppsid = pps.ppsid AND sba.sbastatus = 'A' " . implode(' AND ', $whereIniciativa) . "
                WHERE
                	" . implode(' AND ', $where) . "
                GROUP BY
                	sbd.sbdid,
                    sba.sbaid,
                    pps.ppsid,
                	pps.ppsdsc,
                	pps.ppscronograma,
                    pps.prgid,
                    tor.torid,
		            tor.tordescricao,
                	iuo.iuovalor,
		            sbd.sbdplanointerno,
		            sbd.sbdptres,
                	pts.ptsdescricao
                ORDER BY
                	ppsdsc";

        return $this->pegaLinha($sql);
    }

    public function recuperarItensPospostaIniciativa($dados)
    {
        $sql = "SELECT DISTINCT
                    pic.picid,
                    dic.dicid,
                    ico.icoid,
                    pic.picdescricao,
                    umi.umidescricao,
                    dic.dicvalor
                FROM par.propostaitemcomposicao pic
                INNER JOIN par.unidademedidadetalhamentoitem    umi ON umi.umiid = pic.umiid
                INNER JOIN par.propostasubacaoitem              pit ON pit.picid = pic.picid
                LEFT  JOIN par.detalheitemcomposicao            dic
                	INNER JOIN par.ufdetalheitemcomposicao 		uf  ON uf.dicid = dic.dicid
                	INNER JOIN par3.instrumentounidade		    inu ON inu.estuf = uf.estuf AND inu.inuid = {$dados['inuid']}
                    ON dic.picid = pic.picid AND dicstatus = 'A' AND dic.dicdatainicial <= now()::date AND dic.dicdatafinal >= now()::date
                LEFT  JOIN par.subacaoitenscomposicao           ico ON ico.picid = pic.picid AND ico.dicid = dic.dicid AND ico.icostatus = 'A' AND ico.sbaid = {$dados['sbaid']} AND ico.icoano = 2016
                WHERE
                	pit.ppsid = {$dados['ppsid']}
		            AND dic.dicvalor > 0
		        ORDER BY
		            pic.picdescricao";

        $arrItens = $this->carregar($sql);

        return is_array($arrItens) ? $arrItens : array();
    }

    public function recuperarItensIniciativa($dados)
    {
        $sql = "SELECT
                    ico.icoid,
                	ico.icovalor,
                	ico.icoquantidade,
                	ico.icodescricao
        		FROM par.subacaoitenscomposicao ico
        		WHERE sbaid = {$dados['sbaid']} AND icoano = 2016 AND icostatus = 'A'
		        ORDER BY icodescricao";

        $arrItens = $this->carregar($sql);

        return is_array($arrItens) ? $arrItens : array();
    }

    public function recuperarItensIniciativaEscola($dados)
    {
        $sql = "SELECT
		            ico.sbaid,
                	ico.icoid,
                	ico.icovalor,
                	ico.icoquantidade,
                	ico.icodescricao,
                	SUM(
                	CASE WHEN ses.sesid IS NOT NULL
                	    THEN seiqtd
                	    ELSE 0
                	END
		            ) as qtd_itens
                FROM par.subacaoitenscomposicao ico
                LEFT JOIN par.subescolas_subitenscomposicao sei ON sei.icoid = ico.icoid
                LEFT JOIN par.subacaoescolas                ses ON ses.sesid = sei.sesid AND ses.sesstatus = 'A'
        		WHERE ico.sbaid = {$dados['sbaid']} AND icoano = 2016 AND icostatus = 'A'
		        GROUP BY
                	ico.icoid,
                	ico.icovalor,
                	ico.icoquantidade,
                	ico.icodescricao
                ORDER BY icodescricao";

        $arrItens = $this->carregar($sql);

        return is_array($arrItens) ? $arrItens : array();
    }

    public function recuperarQtdItensIniciativaEscola($dados)
    {
        $sql = "SELECT
                    {$dados['icoid']} as icoid,
                	ses.sesid,
                	mun.mundescricao,
                	esc.escnome,
                	esc.escqtdsalas,
                	esc.escqtdalunos,
                	sei.seiqtd
                FROM par.subacaoescolas ses
                INNER JOIN par.escolas 				            esc ON esc.escid = ses.escid AND escnome IS NOT NULL AND escqtdalunos IS NOT NULL AND escqtdsalas IS NOT NULL
                INNER JOIN entidade.endereco                    ede ON ede.entid = esc.entid
                INNER JOIN territorios.municipio                mun ON mun.muncod = ede.muncod
                LEFT  JOIN par.subescolas_subitenscomposicao	sei ON ses.sesid = sei.sesid AND sei.icoid = {$dados['icoid']}
                WHERE ses.sbaid = {$dados['sbaid']} AND ses.sesano = {$dados['ano']} AND ses.sesstatus = 'A'
                ORDER BY
                    mundescricao,
                    escnome";

        $arrItens = $this->carregar($sql);

        return is_array($arrItens) ? $arrItens : array();
    }

    public function criaIniciativa($dados)
    {
        $modelUnidade = new Par3_Model_InstrumentoUnidade ();
        $inuid_par_antigo = $modelUnidade->recuperarInuidPar($dados ['inuid']);

        $desc = 'Em Elaboração';
        $esdid = 451;

        $sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (62, " . $esdid . ", '" . $desc . "', now()) RETURNING docid ";
        $docid = $this->pegaUm($sql);

        $sql = "INSERT INTO par.subacao(
                	sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao, sbaptres, sbanaturezadespesa, sbamonitoratecnico,
                	frmid, indid, ppsid, prgid, ptsid, sbacronograma, usucpf, sbadataalteracao, sbastatus,
                	sbanovopar, inuid_par3, aciid, docid)
                SELECT DISTINCT
                	pps.ppsdsc,
                	pps.ppsordem,
                	pps.ppsobra,
                	pps.ppsestrategiaimplementacao,
                	pps.ppsptres,
                	pps.ppsnaturezadespesa,
                	pps.ppsmonitora,
                	pps.frmid,
                	pps.indid,
                	pps.ppsid,
                	pps.prgid,
                	pps.ptsid,
                	pps.ppscronograma,
                	'{$_SESSION['usucpf']}' as usucpf,
                	now() as data_alteracao,
                	'A' as status,
                	'S' as sbanovopar,
                	{$dados['inuid']} as inuid_par3,
                	aci.aciid,
                	$docid
                FROM par.propostasubacao 	pps
                INNER JOIN par.propostatiposubacao pts ON pts.ptsid = pps.ptsid AND pts.ptsstatus = 'A'
            	INNER JOIN par.criterio            crt ON crt.indid = pps.indid
            	LEFT  JOIN par.pontuacao           pto
            		INNER JOIN par.acao	aci ON aci.ptoid = pto.ptoid
            		ON pto.crtid = crt.crtid AND pto.inuid = $inuid_par_antigo AND ptostatus = 'A'
                WHERE
                	pps.ppsid = {$dados['ppsid']}
                ORDER BY
                	ppsdsc,
                	aciid ASC
                LIMIT 1
                RETURNING
                    sbaid";

        $sbaid = $this->pegaUm($sql);
        $this->commit();

        return $sbaid;
    }

    public function criaIniciativaDetalhe($dados)
    {
        $sql = "INSERT INTO par.subacaodetalhe(sbaid, sbdano, ssuid)
                VALUES ({$dados['sbaid']}, '2016', 12)
                RETURNING sbdid;";

        $sbdid = $this->pegaUm($sql);
        $this->commit();

        return $sbdid;
    }

    public function buscaItemComposicao($dados)
    {
        $where = array(
            '1=1'
        );
        foreach ($dados as $k => $dado) {
            switch ($k) {
                case 'sbaid' :
                    $where [] = "sbaid = {$dados['sbaid']}";
                    break;
                case 'icoano' :
                    $where [] = "icoano = {$dados['icoano']}";
                    break;
                case 'picid' :
                    $where [] = "picid = {$dados['picid']}";
                    break;
                case 'dicid' :
                    $where [] = "dicid = {$dados['dicid']}";
                    break;
                case 'icoid' :
                    $where [] = "icoid = {$dados['icoid']}";
                    break;
            }
        }

        $sql = "SELECT *
		        FROM par.subacaoitenscomposicao
		        WHERE " . implode(' AND ', $where);

        $arrItem = $this->pegaLinha($sql);

        return $arrItem;
    }

    public function inativaItemComposicao($icoid)
    {
        $sql = "UPDATE par.subacaoitenscomposicao SET icostatus = 'I' WHERE icoid = $icoid";

        $this->executar($sql);
        $this->commit();
    }

    public function ativaItemComposicao($icoid)
    {
        $sql = "UPDATE par.subacaoitenscomposicao SET icostatus = 'A' WHERE icoid = $icoid";

        $this->executar($sql);
        $this->commit();
    }

    public function insereItemComposicao($dados)
    {
        $sql = "INSERT INTO par.subacaoitenscomposicao(
                	sbaid,
                	icoano,
                	icodescricao,
                	icovalor,
                	icostatus,
                	icodetalhe,
                	usucpf,
                	dtatualizacao,
                	picid,
                	dicid
                )
                SELECT
                	{$dados['sbaid']} as sbaid,
                	'2016' as icoano,
                	pic.picdescricao,
                	dic.dicvalor,
                	'A' as icostatus,
                	pic.picdetalhe,
                	'{$_SESSION['usucpf']}' as usucpf,
                	now() as data_atualizacao,
                	pic.picid,
                	dic.dicid
                FROM par.propostaitemcomposicao pic
                INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dic.dicstatus = 'A' AND dic.dicdatainicial <= now()::date AND dic.dicdatafinal >= now()::date
                WHERE
                	pic.picid = {$dados['picid']}
                	AND dic.dicid = {$dados['dicid']}";

        $this->executar($sql);
        $this->commit();
    }

    public function salvaQuantidadeItemComposicao($icoid, $icoquantidade)
    {
        $sql = "UPDATE par.subacaoitenscomposicao
		        SET icoquantidade = $icoquantidade
		        WHERE icoid = $icoid";

        $this->executar($sql);
        $this->commit();
    }

    public function salvaQuantidadeItemComposicaoEscola($sesid, $icoid, $icoquantidade)
    {
        $sql = "SELECT sesid
                FROM par.subescolas_subitenscomposicao
                WHERE sesid = $sesid AND icoid = $icoid";

        $teste = $this->pegaUm($sql);

        if ($teste != '') {
            $sql = "UPDATE par.subescolas_subitenscomposicao
    		        SET seiqtd = $icoquantidade
    		        WHERE
                        icoid = $icoid
                        AND sesid = $sesid";
        } else {
            $sql = "INSERT INTO par.subescolas_subitenscomposicao(sesid, icoid, seiqtd)
                    VALUES($sesid, $icoid, $icoquantidade);";
        }

        $this->executar($sql);
        $this->commit();
    }

    public function montaSQLPI($dados)
    {
        $sql = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno || ' - ' || pliano as descricao
				FROM
					par.planointerno
				WHERE pliano >= 2012  AND prgid = " . $dados ['prgid'];

        return $sql;
    }

    public function montaSQLPTRES($dados)
    {
        $sql = "SELECT
    				pliptres as codigo,
    				pliptres as descricao
    			FROM par.planointerno
    			WHERE plinumplanointerno = '{$dados['sbdplanointerno']}'";

        return $sql;
    }

    public function salvaIniciativaDetalhe($dados)
    {
        $sql = "UPDATE par.subacaodetalhe SET
                    ssuid = 12,
		            sbdplanointerno = '{$dados['sbdplanointerno']}',
                    sbdptres = '{$dados['sbdptres']}'
                WHERE
                    sbdid = {$dados['sbdid']}";

        $this->executar($sql);
        $this->commit();
    }

    public function recuperarEscolasMunicipio($objUnidade, $dados)
    {
        if (in_array($objUnidade->itrid, array(
                1,
                3
            )) && $dados ['muncod'] && $dados ['sbaid']) {

            $sql = "SELECT
    		            ses.escid as checked,
    					esc.escid,
    					m.mundescricao,
    					e.entcodent,
    					e.entnome,
    					e.entid,
    					CASE WHEN ent.pk_cod_entidade IS NOT NULL
    					    THEN count( distinct fk_cod_aluno )
    					    ELSE dec.num_alunos_existentes
    					END as qtde_alunos,
    					CASE WHEN ent.pk_cod_entidade IS NOT NULL
    					    THEN ted.num_salas_existentes
                            ELSE dec.num_salas_existentes
    					END as qtd_salas
    				FROM
    					entidade.entidade e
    				INNER JOIN entidade.funcaoentidade              f   ON f.entid = e.entid AND f.funid = 3
    	            INNER JOIN entidade.endereco                    ed  ON ed.entid = e.entid
    				INNER JOIN territorios.municipio                m   ON m.muncod = ed.muncod
    				LEFT  JOIN par.dadoseducacenso2015              dec ON dec.cod_entidade::varchar = e.entcodent AND dec.fk_cod_municipio::numeric IN (1{$dados['muncod']})
    				LEFT  JOIN par.escolas                          esc ON esc.entid = e.entid AND escnome IS NOT NULL AND escqtdalunos IS NOT NULL AND escqtdsalas IS NOT NULL
    				LEFT  JOIN par.subacaoescolas                   ses ON ses.escid = esc.escid AND ses.sesstatus = 'A' AND ses.sesano = '2016' AND ses.sbaid = '{$dados['sbaid']}'
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_entidade    ent ON e.entcodent = ent.pk_cod_entidade::character varying AND ent.fk_cod_municipio::numeric IN (1{$dados['muncod']})
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_dado_escola ted ON ted.fk_cod_entidade = ent.pk_cod_entidade
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_matricula   mat ON mat.fk_cod_entidade = ent.pk_cod_entidade
    				WHERE
    					(e.entescolanova = false OR e.entescolanova IS NULL)
    					AND e.entstatus = 'A'
    					AND e.tpcid = 1
    					--AND m.estuf = '{$dados['estuf']}'
    					AND m.muncod::numeric IN (1{$dados['muncod']})
    					AND e.entcodent IS NOT NULL
    				GROUP BY
    					m.mundescricao, e.entcodent, ses.escid, e.entid, e.entnome, esc.escid, ted.num_salas_existentes , pk_cod_entidade, dec.num_alunos_existentes, dec.num_salas_existentes
    				ORDER BY
    					m.mundescricao, e.entnome";

            $arrEscolas = $this->carregar($sql);
        } else {
            $sql = "SELECT
    		            ses.escid as checked,
    					esc.escid,
    					m.mundescricao,
    					e.entcodent,
    					e.entnome,
    					e.entid,
    					CASE WHEN ent.pk_cod_entidade IS NOT NULL
    					    THEN count( distinct fk_cod_aluno )
    					    ELSE dec.num_alunos_existentes
    					END as qtde_alunos,
    					CASE WHEN ent.pk_cod_entidade IS NOT NULL
    					    THEN ted.num_salas_existentes
                            ELSE dec.num_salas_existentes
    					END as qtd_salas
    				FROM
    					entidade.entidade e
    				INNER JOIN entidade.funcaoentidade              f   ON f.entid = e.entid AND f.funid = 3
    	            INNER JOIN entidade.endereco                    ed  ON ed.entid = e.entid
    				INNER JOIN territorios.municipio                m   ON m.muncod = ed.muncod
    				LEFT  JOIN par.dadoseducacenso2015              dec ON dec.cod_entidade::varchar = e.entcodent AND dec.fk_cod_municipio::numeric IN (1{$dados['muncod']})
    				LEFT  JOIN par.escolas                          esc ON esc.entid = e.entid AND escnome IS NOT NULL AND escqtdalunos IS NOT NULL AND escqtdsalas IS NOT NULL
    				LEFT  JOIN par.subacaoescolas                   ses ON ses.escid = esc.escid AND ses.sesstatus = 'A' AND ses.sesano = '2016' AND ses.sbaid = '{$dados['sbaid']}'
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_entidade    ent ON e.entcodent = ent.pk_cod_entidade::character varying AND ent.fk_cod_municipio::numeric IN (1{$dados['muncod']})
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_dado_escola ted ON ted.fk_cod_entidade = ent.pk_cod_entidade
    				LEFT  JOIN " . SCHEMAEDUCACENSO . ".tab_matricula   mat ON mat.fk_cod_entidade = ent.pk_cod_entidade
    				WHERE
    					(e.entescolanova = false OR e.entescolanova IS NULL)
    					AND e.entstatus = 'A'
    					AND e.tpcid = 3
    					--AND m.estuf = '{$dados['estuf']}'
    					AND m.muncod::numeric IN (1{$dados['muncod']})
    					AND e.entcodent IS NOT NULL
    				GROUP BY
    					m.mundescricao, e.entcodent, ses.escid, e.entid, e.entnome, esc.escid, ted.num_salas_existentes , pk_cod_entidade, dec.num_alunos_existentes, dec.num_salas_existentes
    				ORDER BY
    					m.mundescricao, e.entnome";

            $arrEscolas = $this->carregar($sql);
        }
        $arrEscolas = is_array($arrEscolas) ? $arrEscolas : array();

        return $arrEscolas;
    }

    public function recuperarEscolasIniciativa($dados)
    {
        $sql = "SELECT
                	esc.escqtdsalas,
                	esc.escqtdalunos,
                	esc.escnome,
                	mun.mundescricao
                FROM par.subacao sba
                INNER JOIN par.subacaodetalhe       sbd ON sbd.sbaid = sba.sbaid AND sbd.sbdano = '2016'
                INNER JOIN par.subacaoescolas       ses ON ses.sbaid = sbd.sbaid AND ses.sesstatus = 'A'
                INNER JOIN par.escolas              esc ON esc.escid = ses.escid AND escnome IS NOT NULL AND escqtdalunos IS NOT NULL AND escqtdsalas IS NOT NULL
                INNER JOIN entidade.entidade        ent ON ent.entid = esc.entid
                INNER JOIN entidade.endereco        ede ON ede.entid = ent.entid
                INNER JOIN territorios.municipio    mun ON mun.muncod = ede.muncod
                WHERE
                	sba.sbaid = {$dados['sbaid']}
                    AND sbd.sbdano = '2016'
                ORDER BY
                    mundescricao,
                    esc.escnome";

        $arrEscolas = $this->carregar($sql);
        $arrEscolas = is_array($arrEscolas) ? $arrEscolas : array();

        return $arrEscolas;
    }

    public function aprovarIniciativas($dados)
    {
        require_once APPRAIZ . 'includes/workflow.php';

        $sql = "SELECT docid FROM par.subacao WHERE inuid_par3 = {$dados['inuid']}";

        $arrDocid = $this->carregarColuna($sql);

        foreach ($arrDocid as $docid) {
            wf_alterarEstado($docid, 1227, "", array(), array());
        }

        $sql = "UPDATE par.subacaodetalhe SET
                	ssuid = 1
                WHERE
                	sbaid IN (
                        SELECT sbaid FROM par.subacao WHERE inuid_par3 = {$dados['inuid']}
                	)";

        $this->executar($sql);
        $this->commit();
    }

    public function reprovarIniciativas($dados)
    {
        require_once APPRAIZ . 'includes/workflow.php';

        $sql = "SELECT docid FROM par.subacao WHERE inuid_par3 = {$dados['inuid']}";

        $arrDocid = $this->carregarColuna($sql);

        foreach ($arrDocid as $docid) {
            wf_alterarEstado($docid, 1260, "", array(), array());
        }

        $sql = "UPDATE par.subacaodetalhe SET
                	ssuid = 7
                WHERE
                	sbaid IN (
                        SELECT sbaid FROM par.subacao WHERE inuid_par3 = {$dados['inuid']}
                	)";

        $this->executar($sql);
        $this->commit();
    }

    /**
     * @param array $arrPost
     * @return array|bool|mixed|NULL
     * @description
     */
    public function getDadosIniciativaDadosDemograficos(array $arrPost)
    {
        $where = $arrPost['iniid'] ? " AND iniciativa.iniid = {$arrPost['iniid']}" : '';
        $sql = "SELECT *
                  FROM {$this->stNomeTabela} as iniciativa
                  LEFT JOIN par3.iniciativa_iniciativas_estados  as iniciativa_iniciativas_estados ON iniciativa_iniciativas_estados.iniid = iniciativa.iniid
                  LEFT JOIN par3.iniciativa_iniciativas_municipios as iniciativas_municipio ON iniciativas_municipio.iniid = iniciativa.iniid
                  LEFT JOIN territorios.estado as estado ON estado.estuf = iniciativa_iniciativas_estados.estuf
                  WHERE inistatus = 'A' {$where}";


        //var_dump($sql); exit();
        return $this->pegaLinha($sql, 0, 0);
    }

    /**
     * @param array $arrPost
     */
    public function retornaEsferaIniciativa(array $arrPost)
    {
        if ($arrPost['iniesfera'] == 'E') {
            return 'estado';
        } elseif ($arrPost['iniesfera'] == 'M') {
            return 'municipio';
        } elseif ($arrPost['iniesfera'] == 'T') {
            return 'todos';

        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param $iniid
     */
    public function getIniciativaById($iniid)
    {
        $sql = "SELECT * FROM par3.iniciativa AS iniciativa
                INNER JOIN par3.iniciativa_descricao AS iniciativa_descricao ON iniciativa_descricao.indid =  iniciativa.indid
                WHERE iniciativa.iniid = {$iniid}";

        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function getIniciativaProinfancia(){
        $sql = "SELECT ini.iniid as codigo, ini.iniid || ' - ' || inddsc as descricao
                FROM par3.iniciativa ini
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                INNER JOIN par3.iniciativa_planejamento inp ON inp.iniid = ini.iniid
                INNER JOIN par3.dimensao dim ON dim.dimid = inp.dimid
                WHERE iniobraproinfancia is true AND inistatus = 'A' AND dimcod = 4";
        return $this->carregar($sql);
    }

    /**
     * @author Djalma Rodrigues <djalma.rodrigues@basis.com.br>
     * @param $request
     * @return string
     */
    public function getSqlRelatorioGerencial($request)
    {
        
        if( $request['itrid'] ) $where .= ( $request['itrid'] == 1 ? " and inu.itrid in (1,3) " : " and inu.itrid = {$request['itrid']} " );
        if( $request['estuf'] ) $where .= " and inu.estuf = '{$request['estuf']}' ";
        if( $request['muncod'] ) $where .= " and inu.muncod = '{$request['muncod']}' ";
        
        if( $request['processogerado'] == "1" ) $where .= " AND pro.pronumeroprocesso IS NOT NULL ";
        if( $request['processogerado'] == "2" ) $where .= " AND pro.pronumeroprocesso IS NULL ";
        if( $request['empenhoefetivado'] == "1" ) $where .= " AND vse.vlr_empenho IS NOT NULL ";
        if( $request['empenhoefetivado'] == "2" ) $where .= " AND vse.vlr_empenho IS NULL ";
        if( $request['termogerado'] == "1" ) $where .= " AND dt.dotnumero IS NOT NULL ";
        if( $request['termogerado'] == "2" ) $where .= " AND dt.dotnumero IS NULL ";
        
        if( $request['dimcod'] ) $where .= " and dim.dimcod = '{$request['dimcod']}' ";
        if( $request['dotid'] ) $where .= " and dt.dotid in (".implode(',', $request['dotid']).") ";
        
        if($request['pronumeroprocesso']) {
            $request['pronumeroprocesso'] = str_replace(".","", $request['pronumeroprocesso']);
            $request['pronumeroprocesso'] = str_replace("/","", $request['pronumeroprocesso']);
            $request['pronumeroprocesso'] = str_replace("-","", $request['pronumeroprocesso']);
            $where .= " and pro.pronumeroprocesso = '".$request['pronumeroprocesso']."'";
        }
        
        if($request['pagamentosituacao']){
            $where .= $request['pagsituacaopagamento'] && $request['pagsituacaopagamento'] != 'sem_pagamento'?
            " AND pag.pagsituacaopagamento ilike '%{$request['pagsituacaopagamento']}%'":' AND pag.pagsituacaopagamento IS NULL';
            
            $join = " LEFT JOIN(
                		select sum(pp.pagvalorparcela) as valor, e.empnumeroprocesso,pp.pagsituacaopagamento
                		from par3.pagamento pp
                			left join par3.empenho e on e.empid = pp.empid and e.empstatus = 'A'
                		where pp.pagstatus = 'A'
                			and e.empcodigoespecie = '01'
                			and pp.pagsituacaopagamento not ilike '%CANCELADO%'
                		    and pp.pagsituacaopagamento not ilike '%vala%'
                			and pp.pagsituacaopagamento not ilike '%devolvido%'
                		    and pp.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
                		group by e.empnumeroprocesso,pp.pagsituacaopagamento
                	) pag on pag.empnumeroprocesso = pro.pronumeroprocesso ";
        }
        
        if($request['intaid'] ){
            switch ($request['intaid'] ){
                case '1':
                    $where .= "
                        AND inp.inpid not in (
                            SELECT inpid FROM par3.iniciativa_emenda ine
                        	inner join par3.analise ana using(inpid)
                        	inner join emenda.emendadetalheentidade ede on ede.edeid = ana.edeid
                           	where ine.inestatus = 'A'
                            and ine.inevalor > 0
                            and ana.anastatus = 'A'
                            and ede.edestatus = 'A'
                            and ana.edeid is not null
                            and ine.edeid is not null
                            --and ine.inpid = pp.inpid
                        ) \n";
                    break;
                case '2':
                    $where .= "
                        AND inp.inpid in (
                        SELECT inpid FROM par3.iniciativa_emenda ine
                        	inner join par3.analise ana using(inpid)
                        	inner join emenda.emendadetalheentidade ede on ede.edeid = ana.edeid
                           	where ine.inestatus = 'A'
                            and ine.inevalor > 0
                            and ana.anastatus = 'A'
                            and ede.edestatus = 'A'
                            and ana.edeid is not null
                            and ine.edeid is not null
                            --and ine.inpid = pp.inpid
                        ) \n";
                    break;
            }
        }
        
        # Por obras
        if ($request['tipo_processo'] == 'O' ) {

            if( $request['octid'] ){
                $request['octid'] = array_filter( $request['octid'], function($val){
                    return !empty($val);
                });
            }
            if( $request['localO'] ){
                $request['localO'] = array_filter( $request['localO'], function($val){
                    return !empty($val);
                });
            }
            if( $request['esdidO'] ){
                $request['esdidO'] = array_filter( $request['esdidO'], function($val){
                    return !empty($val);
                });
            }
            
            if( $request['ano'] ) $where .= " and obr.obrano = '{$request['ano']}' ";
            if( $request['localO'] ) $where .= " and otp.otpid in (".implode(',', $request['localO']).") ";
            if( $request['esdidO'] ) $where .= " and est.esdid in (".implode(',', $request['esdidO']).") ";
            if( $request['octid'] ) $where .= " and oct.octid in (".implode(',', $request['octid']).") ";
            
            $sqlPrincipal = <<<DML
                    select * from(
                    SELECT DISTINCT
                    	inu.estuf AS "UF",
                    	CASE inu.itrid WHEN 1 THEN '-' ELSE inu.inudescricao END AS "Município",
                    	dim.dimcod||' - '||dim.dimdsc AS "Dimensão",
                    	inp.inpid AS "Código de planejamento",
                        obr.obrid||' - '||obr.obrdsc AS "Obra",
                        est.esddsc AS "Situação",
                        obr.obrano AS "Ano",
                        oct.octdsc AS "Categoria",
                        CASE WHEN otp.otpid IN (1, 2, 19, 20, 21, 23) THEN 'Creche'
                            WHEN otp.otpid IN (13, 14, 25) THEN 'Cobertura'
                    		WHEN otp.otpid IN (4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 22) THEN 'Escola'
                            WHEN otp.otpid IN (12, 24) THEN 'Quadra'
                    	ELSE '-' END AS "Local",
                    	odo.tot_documento AS "Documentos",
                    	obr.obrvalor AS "Valor Planejado",
                    	vse.vlr_empenho AS "Valor Empenhado",
                    	vse.vlr_pago AS "Valor Pago",
                    	dt.dotnumero AS "Termo",
                    	formata_num_processo(pro.pronumeroprocesso) AS "Processo"
                    FROM par3.obra obr
                    	INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = obr.inpid AND inp.inpstatus = 'A'
                    	INNER JOIN par3.instrumentounidade inu ON inu.inuid = inp.inuid AND inustatus = 'A'
                    	INNER JOIN workflow.documento doc ON doc.docid = obr.docid
                    	INNER JOIN workflow.estadodocumento est ON doc.esdid = est.esdid
                    	INNER JOIN par3.dimensao dim ON inp.dimid = dim.dimid
                    	INNER JOIN par3.obra_tipo otp ON otp.otpid = obr.otpid
                    	INNER JOIN par3.obra_categoria oct ON oct.octid = otp.octid
                    	LEFT JOIN(
                    		SELECT obrid, count(odoid) AS tot_documento FROM par3.obra_documentos WHERE odostatus = 'A' GROUP BY obrid
                    	) odo ON odo.obrid = obr.obrid
                    	LEFT JOIN(
                    		SELECT obrid, sum(saldo) AS vlr_empenho, sum(vlrpago) AS vlr_pago
                    		FROM par3.v_saldo_empenho_por_obra ve
                    		GROUP BY obrid
                    	) vse ON vse.obrid = obr.obrid
                    	LEFT JOIN par3.processoobracomposicao ppc ON ppc.obrid = obr.obrid AND ppc.pocstatus = 'A'
                    	LEFT JOIN par3.processo pro ON pro.proid = ppc.proid
                    	LEFT JOIN par3.documentotermo dt ON dt.proid = ppc.proid AND dt.dotstatus = 'A'
                        $join
                    WHERE obr.obrstatus = 'A' $where
                    ) as foo
DML;
        # Iniciativas
        } else {
            
            if( $request['esdidI'] ){
                $request['esdidI'] = array_filter( $request['esdidI'], function($val){
                    return !empty($val);
                });
            }
            if( $request['intoid'] ){
                $request['intoid'] = array_filter( $request['intoid'], function($val){
                    return !empty($val);
                });
            }
            
            if( $request['ano'] ) $where .= " and iia.iniano = '{$request['ano']}' ";
            if( $request['esdidI'] ) $where .= " and est.esdid in (".implode(',', $request['esdidI']).") ";
            if( $request['intoid'] ) $where .= " and obj.intoid in (".implode(',', $request['intoid']).") ";
            
            $sqlPrincipal = <<<DML
                     WITH dados_planejamento AS (
                    	SELECT inpid, ipiano, sum(ipivalorreferencia) AS ipivalorreferencia, sum(ipiquantidade) AS ipiquantidade, sum(valor_referencia) AS valor_referencia FROM(
                    		SELECT ipi.inpid, ipi.ipiid, ipi.ipivalorreferencia, ipi.ipiquantidade, ipi.ipiano, (ipi.ipivalorreferencia * ipi.ipiquantidade) AS valor_referencia
                    		FROM par3.iniciativa_planejamento_item_composicao ipi
                    		WHERE ipi.ipistatus = 'A' AND ipiquantidade > 0
                    	) AS foo GROUP BY inpid, ipiano
                    ),
                    dados_analise AS (
                    	SELECT foo.inpid, foo.anaano, sum(aicqtdaprovado) AS aicqtdaprovado, sum(aicvaloraprovado) AS aicvaloraprovado, sum(valor_aprovado) AS valor_aprovado
                    	FROM(
                    		SELECT a.anaid, a.inpid, a.anaano, a.docid, ai.ipiid, ai.aicaprovado, ai.aicqtdaprovado, ai.aicvaloraprovado,
                    			(ai.aicqtdaprovado * ai.aicvaloraprovado) AS valor_aprovado
                    		FROM par3.analise a
                    			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                    		WHERE a.anastatus = 'A'
                    	) AS foo
                    	GROUP BY foo.inpid, foo.anaano
                    ),
                    dados_empenho_pagamento AS (
                    	SELECT ve.ppcano, ve.inpid, sum(ve.saldo) AS vlr_empenho, sum(ve.vlrpago) AS vlr_pago
                    	FROM par3.v_saldo_empenho_composicao_iniciativa ve WHERE ve.saldo > 0
                    	GROUP BY ve.inpid, ve.ppcano
                    )
                    SELECT
                    	inu.estuf AS "UF",
                    	CASE inu.itrid WHEN 1 THEN '-' ELSE inu.inudescricao END AS "Município",
                    	dim.dimcod||' - '||dim.dimdsc AS "Dimensão",
                    	inp.inpid AS "Código de planejamento",
                        ini.iniid||' - '||ind.inddsc AS "Iniciativa",
                        iia.iniano AS "Ano",
                        est.esddsc AS "Situação",
                        obj.intodsc AS "Tipo de objeto",
                        inta.intadsc AS "Tipo de Atendimento",
                    	pla.valor_referencia AS "Valor Planejado",
                    	ana.valor_aprovado AS "Valor Aprovado",
                    	vse.vlr_empenho AS "Valor Empenhado",
                    	vse.vlr_pago AS "Valor Pago",
                    	dt.dotnumero AS "Termo",
                    	formata_num_processo(pro.pronumeroprocesso) AS "Processo"
                    FROM par3.iniciativa_planejamento inp
                    	INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                    	INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
                    	INNER JOIN par3.iniciativa_iniciativas_anos iia ON iia.iniid = ini.iniid
                    	INNER JOIN par3.instrumentounidade inu ON inu.inuid = inp.inuid AND inustatus = 'A'
                    	INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                    	INNER JOIN workflow.estadodocumento est ON doc.esdid=est.esdid
                    	INNER JOIN par3.dimensao dim ON inp.dimid = dim.dimid
                    	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = ini.intoid
                    	INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
                    	LEFT JOIN dados_planejamento pla ON pla.inpid = inp.inpid AND pla.ipiano = iia.iniano
                    	LEFT JOIN dados_analise ana ON ana.inpid = inp.inpid AND ana.anaano = iia.iniano
                    	LEFT JOIN dados_empenho_pagamento vse ON vse.inpid = inp.inpid AND vse.ppcano = iia.iniano
                    	LEFT JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ppc.ppcano = iia.iniano AND ppc.ppcstatus = 'A'
                    	LEFT JOIN par3.processo pro ON pro.proid = ppc.proid
                    	LEFT JOIN par3.documentotermo dt ON dt.proid = ppc.proid AND dt.dotstatus = 'A'
                    	$join
                    WHERE inp.inpstatus = 'A' $where
DML;
        }

        return $sqlPrincipal;
    }

     public function getSqlTotalizadorRelatorioGerencial( $request, $sqlPrincipal)
     {
         if ($request['tipo_processo'] == 'O' ) { // por obras
             $colunasT = 'COALESCE(SUM(COALESCE("Documentos", 0)), 0) AS "Total da quantidade de documentos", SUM("Valor Planejado") AS "Total do Valor Planejado", SUM("Valor Empenhado") AS "Total do Valor Empenhado", sum("Valor Pago") as "Valor Pago"';
             $agrupadoresT = ', obr.obrid, pro.proid';

         } else { // por iniciativas
             $colunasT = 'SUM("Valor Planejado") AS "Total do Valor Planejado", SUM("Valor Empenhado") AS "Total do Valor Empenhado", sum("Valor Aprovado") as "Total Aprovado", sum("Valor Pago") as "Valor Pago"';
         }
         $sqlTotal = str_replace('/*ignorarNoTotalizador*/', '--', $sqlPrincipal);
         $sqlTotal = <<<DML
WITH selecao AS (
$sqlTotal
)
SELECT $colunasT FROM selecao
DML;
         return $sqlTotal;
     }

     function getDadosSolicitacaoMobiliario($itrid){
         $sql = "SELECT dimid FROM par3.dimensao WHERE itrid = {$itrid} AND dimcod = 4";
         return $this->pegaUm($sql);
     }

     function verificaIniciativaProinfancia(){
         $sql = 'SELECT iniid FROM par3.iniciativa WHERE iniobraproinfancia IS TRUE AND inistatus = \'A\' ';
         return $this->pegaUm($sql);
     }


} // end class

?>