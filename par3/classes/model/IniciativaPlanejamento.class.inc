<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Iniciativa_planejamento: Tabela utilizada para cadastro do Planejamento das Iniciativas
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $inpid Chave Primária - default: nextval('par3.iniciativa_planejamento_inpid_seq'::regclass)
 * @property int $dimid FK par3.dimensao
 * @property int $inuid FK par3.instrumentounidade
 * @property int $iniid FK par3.iniciativa
 * @property int $imoid
 * @property int $docid
 * @property string $inpstatus Status (SOFT DELETE) - default: 'A'::bpchar
 * @property string $inpsituacaocadastro  - default: 'A'::bpchar
 */
class Par3_Model_IniciativaPlanejamento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'inpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
        'dimid' => array('tabela' => 'par3.dimensao', 'pk' => 'dimid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),

    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'inpid' => null,
        'dimid' => null,
        'iniid' => null,
        'docid' => null,
        'inpstatus' => null,
        'inpsituacaocadastro' => null,
        'inuid' => null,
        'inpcronogramamesinicial' => null,
        'inpcronogramamesfinal' => null,
        'inpcronogramaano' => null,
        'inpitemcomposicaodetalhamento' => null,
        'nivid' => null,
        'etaid' => null,
        'modid' => null,
        'inpcronogramaanoinicial' => null,
        'inpvalor_planejado_total' => null,
        'obrid' => null,
        'obrid_par3' => null,
    );
    const TIPO_INICIATIVA = '302';

//    /**
//     * Validators.
//     *
//     * @param mixed[] $dados
//     * @return mixed[]
//     */
//    public function getCamposValidacao($dados = array())
//    {
//        return array(
//            'inpid' => array('Digits'),
//            'dimid' => array('Digits'),
//            'iniid' => array('allowEmpty' => true,'Digits'),
//            'imoid' => array('Digits'),
//            'docid' => array('Digits'),
//            'inpstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
//            'inpsituacaocadastro' => array(new Zend_Validate_StringLength(array('max' => 1))),
//        );
//    }
//
//    /**
//     * Método de transformação de valores e validações adicionais de dados.
//     *
//     * Este método tem as seguintes finalidades:
//     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
//     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
//     *
//     * @return bool
//     */
//    public function antesSalvar()
//    {
//        // -- Implemente suas transformações de dados aqui
//
//        // -- Por padrão, o método sempre retorna true
//        return parent::antesSalvar();
//

    public function retornaCategoriaIniciativa()
    {
        $sql = "SELECT
                    oct.octid,
                    oct.octdsc
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa ini ON  ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_tipo_obra ipo ON ipo.iniid = inp.iniid
                INNER JOIN par3.obra_tipo otp ON otp.otpid = ipo.otpid
                INNER JOIN par3.obra_categoria oct on oct.octid = otp.octid
                WHERE
    	           inp.inpid = {$this->inpid}
                ORDER BY
                    oct.octid";

        return $this->pegaLinha($sql);
    }

    public function validarInput($campos)
    {
        $arrDes = array();
        if ($campos['desid']) {
            foreach ($campos['desid'] as $des) {
                if ($des == '') {
                    continue;
                }
                $arrDes[] = $des;
            }
        }

        //campos
        $erros['erros']['dimid'] = array();
        $erros['erros']['nivid'] = array();
        $erros['erros']['iniid'] = array();
        $erros['erros']['etaid'] = array();
        $erros['erros']['modid'] = array();
        $erros['erros']['desid'] = array();
        $erros['erros']['inpcronogramamesinicial'] = array();
        $erros['erros']['inpcronogramaano'] = array();

        //VALIDA CAMPOS
        if (!$campos['inpid']) {
            if ($campos['dimid'] == '' || $campos['dimid'] == null) {
                array_push($erros['erros']['dimid'], 'Informe uma Dimensão');
            }
            if ($campos['iniid'] == '' || $campos['iniid'] == null) {
                array_push($erros['erros']['iniid'], 'Informe Uma Iniciativa');
            }
            if ($campos['etaid'] == '' || $campos['etaid'] == null) {
                array_push($erros['erros']['etaid'], 'Informe uma Etapa');
            }
            if ($campos['modid'] == '' || $campos['modid'] == null) {
                array_push($erros['erros']['modid'], 'Informe uma Modalidade');
            }
            if (empty($arrDes)) {
                array_push($erros['erros']['desid'], 'Informe um Desdobramento');
            }
        }

        if ($arrDes) {
        };
        if ($campos['inpcronogramamesinicial'] . $campos['inpcronogramamesfinal'] . $campos['inpcronogramaano'] != '') {
            if (($campos['inpcronogramamesinicial'] == '' || $campos['inpcronogramamesinicial'] == null) && ($campos['inpcronogramamesfinal'] == '' || $campos['inpcronogramamesfinal'] == null) && ($campos['inpcronogramaano'] == '' || $campos['inpcronogramaano'] == null)) {
                array_push($erros['erros']['inpcronogramamesinicial'], 'Todos os campos do cronograma são obrigatórios.');
            } else {
//                if($campos['inpcronogramamesinicial'] > $campos['inpcronogramamesfinal']){
//                    array_push($erros['erros']['inpcronogramamesinicial'], 'O mês inicial deve ser menor ou igual ao final.');
//                }
                if ($campos['inpcronogramaano'] > 3000 || $campos['inpcronogramaano'] < 2000) {
                    array_push($erros['erros']['inpcronogramaano'], 'Ano inválido.');
                }
            }
        }

        $arrTipoDes = array();
        foreach ($arrDes as $desid) {
            $mdDes = new Par3_Model_IniciativaDesdobramentos($desid);
            if (!in_array($mdDes->tipid, $arrTipoDes)) {
                $arrTipoDes[] = $mdDes->tipid;
            } else {
                array_push($erros['erros']['desid'], 'Não pode escolher mais de um desdobramento do mesmo tipo.');
            }
        }

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value) {
            if (!empty($erros['erros'][$key])) {
                return $erros;
            }
        }
        return false;
    }


    public function montarSQLDataGrid($arrPost)
    {
        $inddsc = sanitizar_string_pesquisa($arrPost['inddsc']);
        $inpid = $arrPost['codigo'];

        $where = $arrPost['inuid'] ? " AND inp.inuid = {$arrPost['inuid']}" : '';
        $where .= $arrPost['inddsc'] ? " AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' " : '';
        $where .= $inpid && validarInteiro($inpid) ? "  AND inp.inpid = {$inpid} " : '';
        $where .= $arrPost['cicid'] && validarInteiro($arrPost['cicid']) ? "AND ini.cicid = {$arrPost['cicid']}" : "";
        $where .= $arrPost['dimid'] && validarInteiro($arrPost['dimid']) ? "AND inp.dimid = {$arrPost['dimid']}" : "";
        $where .= $arrPost['situacao'] ? " AND esd.esdid = {$arrPost['situacao']}" : '';
        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}" : " ";
        $contador = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls' ? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';
        $inicio = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (" : '';
        $fim = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls' ? ") as foo" : '';


        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}" : " ORDER BY inp.inpid DESC";

        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         * $arrPost['inuid']
         */
        $sql = "{$inicio}
                SELECT DISTINCT
                    inp.inpid  as id,
                    inp.inpid  as codigo,
                   -- (dim.dimcod||' - '||dim.dimdsc) as dimensao,
                    dim.dimcod as dimensao,
                    (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        prg.prgdsc
                        FROM par3.iniciativa_iniciativas_programas iiprg
                        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as programas,

                    obj.intodsc as tipoobjeto,

                    --inp.iniid    as iniciativa,
                    eta.etadsc   as etapa,
                    mod.moddsc   as modalidade,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        des.desdsc
                        FROM par3.iniciativa_planejamento_desdobramentos ipd
                        INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                        WHERE ipd.inpid = inp.inpid
                    ),',')
                    ) as desdobramentos,

                    ind.inddsc   as descricao,
                    niv.nivdsc   as nivel,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        iniano
                        FROM par3.iniciativa_iniciativas_anos
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as anos,

                    cic.cicdsc   as ciclo,
                    inta.intadsc as tipoatendimento,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        iar.iarsigla||' - '||iar.iardsc
                        FROM par3.iniciativa_iniciativas_areas iiar
                        INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iiar.iarid
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as areas,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        pro.prodsc
                        FROM par3.iniciativa_iniciativas_projetos iipro
                        INNER JOIN par3.projeto pro ON pro.proid = iipro.proid
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as projetos,

                    (CASE (inp.inpcronogramamesinicial)
                        WHEN '1'  THEN 'Janeiro'
                        WHEN '2'  THEN 'Fevereiro'
                        WHEN '3'  THEN 'Março'
                        WHEN '4'  THEN 'Abril'
                        WHEN '5'  THEN 'Maio'
                        WHEN '6'  THEN 'Junho'
                        WHEN '7'  THEN 'Julho'
                        WHEN '8'  THEN 'Agosto'
                        WHEN '9'  THEN 'Setembro'
                        WHEN '10' THEN 'Outubro'
                        WHEN '11' THEN 'Novembro'
                        WHEN '12' THEN 'Dezembro'
                        ELSE ''
                        END
                    ) as cronoinicio,

                    (CASE (inp.inpcronogramamesfinal)
                        WHEN '1'  THEN 'Janeiro'
                        WHEN '2'  THEN 'Fevereiro'
                        WHEN '3'  THEN 'Março'
                        WHEN '4'  THEN 'Abril'
                        WHEN '5'  THEN 'Maio'
                        WHEN '6'  THEN 'Junho'
                        WHEN '7'  THEN 'Julho'
                        WHEN '8'  THEN 'Agosto'
                        WHEN '9'  THEN 'Setembro'
                        WHEN '10' THEN 'Outubro'
                        WHEN '11' THEN 'Novembro'
                        WHEN '12' THEN 'Dezembro'
                        ELSE ''
                        END
                    ) as cronofim,
                    inp.inpcronogramaano as cronoano,

                    COALESCE(inpvalor_planejado_total,0) as valor,
                    esd.esddsc as situacao,
                    esd.esdid as esdid

                FROM {$this->stNomeTabela} as inp
                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                LEFT JOIN workflow.documento doc ON doc.docid = inp.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
                INNER JOIN par3.ensino_nivel niv ON niv.nivid = inp.nivid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.modalidade   mod ON mod.modid = inp.modid

                INNER JOIN par3.ciclo_par                    cic  ON cic.cicid   = ini.cicid
                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
                WHERE inpstatus = 'A'
                    {$where}
                    {$orderBy}
                    {$fim}";
        //   ver($sql);
        return $sql;
    }

    public function montarSqlDimensao($arrPost)
    {
        $inddsc = sanitizar_string_pesquisa($arrPost['inddsc']);

        $innerJoinEscolas = "";
        $where = $arrPost['inuid'] ? " AND inp.inuid = {$arrPost['inuid']}" : '';
        $where .= $arrPost['inddsc'] ? " AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' " : '';
        $where .= $arrPost['ano'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT iniano FROM par3.iniciativa_iniciativas_anos WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['ano']}%' " : '';
        $where .= $arrPost['codigo'] ? " AND inp.inpid  = {$arrPost['codigo']}" : '';
        $where .= $arrPost['tipoobjeto'] ? " AND  obj.intoid = {$arrPost['tipoobjeto']}" : '';
        $where .= $arrPost['tipoatendimento'] ? " AND   ini.intaid = {$arrPost['tipoatendimento']}" : '';
        $where .= $arrPost['programas'] ? " AND (SELECT array_to_string(array( SELECT DISTINCT prg.prgid FROM par3.iniciativa_iniciativas_programas iiprg
                                                 INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                                                 WHERE iniid = ini.iniid
                                                  ),',')) LIKE '%{$arrPost['programas']}%'" : '';
        $where .= $arrPost['areas'] ? " AND  iniiar.iarid = {$arrPost['areas']}" : '';
        $where .= $inpid && validarInteiro($inpid) ? "  AND inp.inpid = {$inpid} " : '';
        $where .= $arrPost['cicid'] && validarInteiro($arrPost['cicid']) ? "AND ini.cicid = {$arrPost['cicid']}" : "";
        $where .= $arrPost['dimid'] && validarInteiro($arrPost['dimid']) ? "AND inp.dimid = {$arrPost['dimid']}" : "";
        $where .= $arrPost['situacao'] ? " AND esd.esdid = {$arrPost['situacao']}" : '';
        $where .= $arrPost['etapas'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT   ee.etaid FROM par3.iniciativa_modalidades inimdl
                                             INNER JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
                                             WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['etapas']}%' " : '';
        $where .= $arrPost['desdobramento'] ? " AND ( SELECT DISTINCT des.desid FROM par3.iniciativa_planejamento_desdobramentos ipd
                                                INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                                                WHERE ipd.inpid = inp.inpid
             		                             )  = {$arrPost['desdobramento']} " : '';
        $where .= $arrPost['escola'] ? " AND esc.escid = {$arrPost['escola']} and esc.escco_situacao_funcoinamento in(1,2)" : '';
        if ($arrPost['escola']) {
            $innerJoinEscolas = "INNER join par3.iniciativa_planejamento_item_composicao ipi on ipi.inpid = inp.inpid
                            INNER join par3.iniciativa_planejamento_item_composicao_escola esc on esc.ipiid = ipi.ipiid
                            inner join  par3.escola esc1 on esc1.escid = esc.escid and esc1.escco_situacao_funcoinamento in(1,2)";
        }

        $inicio = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (" : '';
        $fim = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls' ? ") as foo" : '';


        $sql = "{$inicio}
               SELECT DISTINCT
                    
                     dim.dimcod,
                    (dim.dimcod||' - '||dim.dimdsc) as dimensao,
           	(SELECT DISTINCT
   				count(inp.inpid)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =   dim.dimcod ) as iniciativasplanejadas,
                        
           	(SELECT DISTINCT
   				count(inp.inpid)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                        INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =  dim.dimcod
                        --AND  esd.esdid = 1991 
                          AND  esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") 
                       ) as iniciativasfinalizadas,
                       
                        	(SELECT DISTINCT
   				COALESCE(sum(inp.inpvalor_planejado_total),0)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                        INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =  dim.dimcod
                        --AND esd.esdid = 1991
                         AND esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") 
                       ) as valortitaldimensao
                       
                FROM {$this->stNomeTabela} as inp
              
                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
                INNER JOIN par3.ensino_nivel niv ON niv.nivid = inp.nivid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.modalidade   mod ON mod.modid = inp.modid
            	INNER JOIN par3.iniciativa_tipos_atendimento as inita ON inita.intaid = ini.intaid
            	INNER JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iniid = ini.iniid 
            	INNER JOIN par3.iniciativa_areas as iniar ON iniar.iarid = iniiar.iarid
                INNER JOIN par3.ciclo_par                    cic  ON cic.cicid   = ini.cicid
                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                {$innerJoinEscolas}
 
                WHERE inpstatus = 'A'
                    {$where}
                        ORDER BY dimcod
                    {$fim}";

        return $sql;
    }


    public function montarSqlDimensaoImpressao($arrPost)
    {
        $innerJoinEscolas = "";
        $inddsc = sanitizar_string_pesquisa($arrPost['inddsc']);

        $where = $arrPost['inuid'] ? " AND inp.inuid = {$arrPost['inuid']}" : '';
        $where .= $arrPost['inddsc'] ? " AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' " : '';
        $where .= $arrPost['codigo'] ? " AND inp.inpid  = {$arrPost['codigo']}" : '';
        $where .= $arrPost['tipoobjeto'] ? " AND  obj.intoid = {$arrPost['tipoobjeto']}" : '';
        $where .= $arrPost['tipoatendimento'] ? " AND   ini.intaid = {$arrPost['tipoatendimento']}" : '';
        $where .= $arrPost['programas'] ? " AND (SELECT array_to_string(array( SELECT DISTINCT prg.prgid FROM par3.iniciativa_iniciativas_programas iiprg
                                                 INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                                                 WHERE iniid = ini.iniid
                                                  ),',')) LIKE '%{$arrPost['programas']}%'" : '';
        $where .= $arrPost['areas'] ? " AND  iniiar.iarid = {$arrPost['areas']}" : '';
        $where .= $inpid && validarInteiro($inpid) ? "  AND inp.inpid = {$inpid} " : '';
        $where .= $arrPost['cicid'] && validarInteiro($arrPost['cicid']) ? "AND ini.cicid = {$arrPost['cicid']}" : "";
        $where .= $arrPost['dimid'] && validarInteiro($arrPost['dimid']) ? "AND inp.dimid = {$arrPost['dimid']}" : "";
        $where .= $arrPost['situacao'] ? " AND esd.esdid = {$arrPost['situacao']}" : '';
        $where .= $arrPost['etapas'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT   ee.etaid FROM par3.iniciativa_modalidades inimdl
                                             INNER JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
                                             WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['etapas']}%' " : '';
        $where .= $arrPost['desdobramento'] ? " AND ( SELECT DISTINCT des.desid FROM par3.iniciativa_planejamento_desdobramentos ipd
                                                INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                                                WHERE ipd.inpid = inp.inpid
             		                             )  = {$arrPost['desdobramento']} " : '';

        $where .= $arrPost['ano'] ? " AND iniano = {$arrPost['ano']}" : '';
        $where .= $arrPost['escola'] ? " AND esc.escid = {$arrPost['escola']}" : '';


        $where1 = $arrPost['inuid'] ? " AND inp.inuid = {$arrPost['inuid']}" : '';
        $where1 .= $arrPost['inddsc'] ? " AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' " : '';
        $where1 .= $arrPost['ano'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT iniano FROM par3.iniciativa_iniciativas_anos WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['ano']}%' " : '';
        $where1 .= $arrPost['codigo'] ? " AND inp.inpid  = {$arrPost['codigo']}" : '';
        $where1 .= $arrPost['tipoobjeto'] ? " AND  obj.intoid = {$arrPost['tipoobjeto']}" : '';
        $where1 .= $arrPost['tipoatendimento'] ? " AND   ini.intaid = {$arrPost['tipoatendimento']}" : '';
        $where1 .= $arrPost['programas'] ? " AND (SELECT array_to_string(array( SELECT DISTINCT prg.prgid FROM par3.iniciativa_iniciativas_programas iiprg
                                                 INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                                                 WHERE iniid = ini.iniid
                                                  ),',')) LIKE '%{$arrPost['programas']}%'" : '';
        $where1 .= $arrPost['areas'] ? " AND  iniiar.iarid = {$arrPost['areas']}" : '';
        $where1 .= $inpid && validarInteiro($inpid) ? "  AND inp.inpid = {$inpid} " : '';
        $where1 .= $arrPost['cicid'] && validarInteiro($arrPost['cicid']) ? "AND ini.cicid = {$arrPost['cicid']}" : "";
        $where1 .= $arrPost['dimid'] && validarInteiro($arrPost['dimid']) ? "AND inp.dimid = {$arrPost['dimid']}" : "";
        $where1 .= $arrPost['situacao'] ? " AND esd.esdid = {$arrPost['situacao']}" : '';
        $where1 .= $arrPost['etapas'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT   ee.etaid FROM par3.iniciativa_modalidades inimdl
                                             INNER JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
                                             WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['etapas']}%' " : '';
        $where1 .= $arrPost['desdobramento'] ? " AND ( SELECT DISTINCT des.desid FROM par3.iniciativa_planejamento_desdobramentos ipd
                                                INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                                                WHERE ipd.inpid = inp.inpid
             		                             )  = {$arrPost['desdobramento']} " : '';

        $where1 .= $arrPost['escola'] ? " AND esc.escid = {$arrPost['escola']}" : '';

        if ($arrPost['escola']) {
            $innerJoinEscolas = "INNER join par3.iniciativa_planejamento_item_composicao ipi on ipi.inpid = inp.inpid
                            INNER join par3.iniciativa_planejamento_item_composicao_escola esc on esc.ipiid = ipi.ipiid
                            inner join  par3.escola esc1 on esc1.escid = esc.escid and esc.escco_situacao_funcoinamento in(1,2)";
        }

        $sql = "
    SELECT DISTINCT
                    
                     dim.dimcod,
                    (dim.dimcod||' - '||dim.dimdsc) as dimensao,
           	(SELECT DISTINCT
   				count(inp.inpid)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =   dim.dimcod ) as iniciativasplanejadas,
                        
           	(SELECT DISTINCT
   				count(inp.inpid)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                        INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =  dim.dimcod
                        --AND  esd.esdid = 1991 
                         AND esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") 
                       ) as iniciativasfinalizadas,
                       
                        	(SELECT DISTINCT
   				COALESCE(sum(inp.inpvalor_planejado_total),0)
                        FROM par3.iniciativa_planejamento as inp
                        INNER JOIN par3.dimensao dime ON inp.dimid  = dime.dimid
                        INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                        INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        WHERE inpstatus = 'A' 
                        AND inp.inuid = {$arrPost['inuid']}
                        AND dime.dimcod =  dim.dimcod
                        --AND esd.esdid = 1991
                         AND esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") 
                       ) as valortitaldimensao,
                       
                       (SELECT array_to_string(array(SELECT  DISTINCT  ' Ano: '|| iniano ||' <br>'|| 'Descrição: '||(inp.iniid||' - '||ind.inddsc)  ||' <br>'|| 'Objeto: '|| obj.intodsc ||' <br>'|| 'Programa:'||(SELECT array_to_string(array(
                        SELECT DISTINCT
                        prg.prgdsc
                        FROM par3.iniciativa_iniciativas_programas iiprg
                        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                        WHERE iniid = ini.iniid
                    ),',')
                    )||' <br>'|| 'Situação: '||esd.esddsc||' <br>'|| 'Valor: '||(CASE WHEN ini.intoid = 1 
                        THEN 
                            (SELECT coalesce(SUM(obrvalor),0) FROM par3.obra obr WHERE obr.inuid = inp.inuid AND obr.inpid = inp.inpid AND obr.obrano = iniano)
                        ELSE
                        CASE WHEN ini.intaid = 1
                        then
                            ( select coalesce(sum(ipiquantidade * ipivalorreferencia),0) from par3.iniciativa_planejamento_item_composicao ipi        
                               where ipi.inpid = inp.inpid and ipi.ipiano = iniano
                              )
                            ELSE 
                            (
                               select coalesce(sum(esc.ipequantidade * ipi.ipivalorreferencia),0) from par3.iniciativa_planejamento_item_composicao ipi
                               inner join par3.iniciativa_planejamento_item_composicao_escola esc on ipi.ipiid = esc.ipiid
                               where inpid = inp.inpid and ipi.ipiano = iniano
                        )END
                   end )||' <br>'


                FROM par3.iniciativa_planejamento as inp
                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid 
                INNER JOIN par3.iniciativa_iniciativas_anos ina on ina.iniid = ini.iniid
         		INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
         		INNER JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iniid = ini.iniid 
            	INNER JOIN par3.iniciativa_areas as iniar ON iniar.iarid = iniiar.iarid
            	{$innerJoinEscolas}
                WHERE inpstatus = 'A' 
                 AND dim.dimcod = dim.dimcod AND inp.inuid = {$arrPost['inuid']}
                 {$where}
                group by iniano,inp.iniid,ind.inddsc, obj.intodsc,ini.iniid,esd.esdid,inp.inpid
              
           ),'<br>')
                    ) 
                        
                FROM par3.iniciativa_planejamento as inp
              
                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
                INNER JOIN par3.ensino_nivel niv ON niv.nivid = inp.nivid
                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
                INNER JOIN par3.modalidade   mod ON mod.modid = inp.modid
            	INNER JOIN par3.iniciativa_tipos_atendimento as inita ON inita.intaid = ini.intaid
            	INNER JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iniid = ini.iniid 
            	INNER JOIN par3.iniciativa_areas as iniar ON iniar.iarid = iniiar.iarid
                INNER JOIN par3.ciclo_par                    cic  ON cic.cicid   = ini.cicid
                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                {$innerJoinEscolas}
                WHERE inpstatus = 'A'
                     {$where1}
                     order by dim.dimcod
               
              
                    ";

        return $sql;
    }

    public function recuperarTodosEmCadastramento($inuid)
    {
        $sql = "SELECT inp.docid
                FROM {$this->stNomeTabela} inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE inuid = {$inuid} AND inpstatus = 'A'
                AND   esd.esdid = 1990";
        return $this->carregar($sql);
    }



    public function recuperarObrasEmCadastramento($inuid, $inpid)
    {
        $sql = "SELECT obr.docid
                FROM par3.obra obr
                INNER JOIN workflow.documento doc ON doc.docid = obr.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE obr.inuid = {$inuid} AND obr.obrstatus = 'A' and obr.inpid = {$inpid}
                AND   esd.esdid = 2007";
        return $this->carregar($sql);
    }


    public function recuperarTodasFinalizadas($inuid)
    {
        $sql = "SELECT inp.docid
                FROM {$this->stNomeTabela} inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE inuid = {$inuid} AND inpstatus = 'A'
                --AND   esd.esdid = 1991
                AND   esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") ";
        return $this->carregar($sql);
    }

    public function montarSqlIniciativasDimensao($dimcod, $arrPost)
    {
       
        $innerJoinEscolas = "";
        $inddsc = sanitizar_string_pesquisa($arrPost['inddsc']);

        $where = $dimcod ? " AND dim.dimcod = {$dimcod}" : '';

        $where .= $arrPost['inuid'] ? " AND inp.inuid = {$arrPost['inuid']}" : '';
        $where .= $arrPost['inddsc'] ? " AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' " : '';

        $where .= $arrPost['codigo'] ? " AND inp.inpid  = {$arrPost['codigo']}" : '';
        $where .= $arrPost['tipoobjeto'] ? " AND  obj.intoid = {$arrPost['tipoobjeto']}" : '';
        $where .= $arrPost['tipoatendimento'] ? " AND   ini.intaid = {$arrPost['tipoatendimento']}" : '';
        $where .= $arrPost['programas'] ? " AND (SELECT array_to_string(array( SELECT DISTINCT prg.prgid FROM par3.iniciativa_iniciativas_programas iiprg
                                                 INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                                                 WHERE iniid = ini.iniid
                                                  ),',')) LIKE '%{$arrPost['programas']}%'" : '';
        $where .= $arrPost['areas'] ? " AND  iniiar.iarid = {$arrPost['areas']}" : '';
        $where .= $inpid && validarInteiro($inpid) ? "  AND inp.inpid = {$inpid} " : '';
        $where .= $arrPost['cicid'] && validarInteiro($arrPost['cicid']) ? "AND ini.cicid = {$arrPost['cicid']}" : "";
        $where .= $arrPost['dimid'] && validarInteiro($arrPost['dimid']) ? "AND inp.dimid = {$arrPost['dimid']}" : "";
        $where .= $arrPost['situacao'] ? " AND esd.esdid = {$arrPost['situacao']}" : '';
        $where .= $arrPost['etapas'] ? "  AND (SELECT array_to_string(array( SELECT DISTINCT   ee.etaid FROM par3.iniciativa_modalidades inimdl
                                             INNER JOIN par3.ensino_etapa ee ON ee.etaid = inimdl.etaid
                                             WHERE iniid = ini.iniid ),',') )  LIKE '%{$arrPost['etapas']}%' " : '';
        $where .= $arrPost['desdobramento'] ? " AND ( SELECT DISTINCT des.desid FROM par3.iniciativa_planejamento_desdobramentos ipd
                                                INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                                                WHERE ipd.inpid = inp.inpid
             		                             )  = {$arrPost['desdobramento']} " : '';
        $where .= $arrPost['ano'] ? " AND iniano = {$arrPost['ano']}" : '';

        $where .= $arrPost['escola'] ? " AND esc.escid = {$arrPost['escola']}" : '';
        if ($arrPost['escola']) {
            $innerJoinEscolas = "INNER join par3.iniciativa_planejamento_item_composicao ipi on ipi.inpid = inp.inpid
                            INNER join par3.iniciativa_planejamento_item_composicao_escola esc on esc.ipiid = ipi.ipiid
                            inner join  par3.escola esc1 on esc1.escid = esc.escid and esc1.escco_situacao_funcoinamento in(1,2)";
        }

        $sql = "SELECT  DISTINCT
                     inp.inpid as acao,
                     inp.inpid,
                 
                     iniano,
               
                    (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,
                    obj.intodsc as tipoobjeto,
                    
                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        prg.prgdsc
                        FROM par3.iniciativa_iniciativas_programas iiprg
                        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as programas,
                       esd.esddsc as situacao,
                    
		                   
                    (CASE WHEN ini.intoid = 1 
                        THEN 
                            (SELECT coalesce(SUM(obrvalor),0) FROM par3.obra obr WHERE obr.inuid = inp.inuid AND obr.inpid = inp.inpid AND obr.obrano = iniano AND obr.obrstatus = 'A')
                        ELSE
                        CASE WHEN ini.intaid = 1
                        then
                            ( select coalesce(sum(ipiquantidade * ipivalorreferencia),0) from par3.iniciativa_planejamento_item_composicao ipi        
                               where ipi.inpid = inp.inpid and ipi.ipiano = iniano AND  ipi.ipistatus = 'A' 
                              )
                            ELSE 
                            (
                               select coalesce(sum(esc.ipequantidade * ipi.ipivalorreferencia),0) from par3.iniciativa_planejamento_item_composicao ipi
                               inner join par3.iniciativa_planejamento_item_composicao_escola esc on ipi.ipiid = esc.ipiid
                               where inpid = inp.inpid and ipi.ipiano = iniano AND  ipi.ipistatus = 'A' 
                        )END
                   end )AS valor 

                FROM par3.iniciativa_planejamento as inp
                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid 
                INNER JOIN par3.iniciativa_iniciativas_anos ina on ina.iniid = ini.iniid
         		INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
         		INNER JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iniid = ini.iniid 
            	INNER JOIN par3.iniciativa_areas as iniar ON iniar.iarid = iniiar.iarid
                {$innerJoinEscolas}
                WHERE inpstatus = 'A' 
                {$where} 
                group by iniano,inp.iniid,ind.inddsc, obj.intodsc,ini.iniid,esd.esdid,inp.inpid
                order by iniano
                

            ";
        return $sql;
    }


    public function recuperaDocumento()
    {
        if ($this->docid) {
            return $this->docid;
        }
        $docid = wf_cadastrarDocumento(self::TIPO_INICIATIVA, 'Planejamento Iniciativa PAR3');
        return $docid;
    }

    public function recuperarTipoObra()
    {
        $sql = "SELECT
                    oct.octid,oct.octdsc
                    FROM par3.iniciativa_planejamento inp
                    INNER JOIN par3.iniciativa ini ON  ini.iniid = inp.iniid
                    INNER JOIN par3.iniciativa_tipo_obra ipo ON ipo.iniid = inp.iniid
                    INNER JOIN par3.obra_tipo otp ON otp.otpid = ipo.otpid
                    INNER JOIN par3.obra_categoria oct on oct.octid = otp.octid
                    WHERE inp.inpid = {$this->inpid}";
        return $this->recuperar($sql);
    }

    public function getIniciativaPlanejamentoByIdIniciativaDescricao($indid)
    {
        $sql = "SELECT * FROM
                par3.iniciativa_planejamento as iniciativa_planejamento
                INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
                INNER JOIN par3.iniciativa_descricao as iniciativa_descricao ON iniciativa.indid  =  iniciativa_descricao.indid
                WHERE iniciativa_descricao.indid = {$indid}";
        $result = $this->carregar($sql);
        return $result;
    }

    public function recuperarPorIniciativa($iniid)
    {
        $sql = "SELECT  *
                FROM {$this->stNomeTabela}
                WHERE iniid = {$iniid}
                AND inpstatus = 'A'
                ";
        $retorno = $this->recuperar($sql);
        return $retorno;
    }

    public function verificaExistenciaParaSalvar(array $array)
    {
        $and = '';
        $array = array_filter($array);
        $and  = $array['dimid'] ? "AND iniciativa_planejamento.dimid = {$array['dimid']}" : '';
        $and .= $array['iniid'] ? "AND iniciativa_planejamento.iniid = {$array['iniid']}" : '';
        $and .= $array['etaid'] ? "AND iniciativa_planejamento.etaid = {$array['etaid']}" : '';
        $and .= $array['modid'] ? "AND iniciativa_planejamento.modid = {$array['modid']}" : '';
        $sql = "SELECT
                iniciativa_planejamento.inuid,
                iniciativa_planejamento.iniid,
                iniciativa_planejamento.etaid,
                iniciativa_planejamento.modid
                FROM par3.iniciativa_planejamento AS iniciativa_planejamento
                INNER JOIN par3.dimensao AS dimensao ON dimensao.dimid = iniciativa_planejamento.dimid
                INNER JOIN par3.ensino_etapa AS ensino_etapa ON ensino_etapa.etaid = iniciativa_planejamento.etaid
                INNER JOIN par3.modalidade AS modalidade ON modalidade.modid = iniciativa_planejamento.modid
                INNER JOIN par3.iniciativa AS iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
                WHERE
                    iniciativa_planejamento.inuid = {$array['inuid']}
                    {$and}
                AND iniciativa_planejamento.inpstatus = 'A' ";
        return $this->carregar($sql);
    }

    public function verificaExistenciaPendenciasPreenchimento(array $array)
    {
        global $db;
        $sql = "    SELECT DISTINCT
                    count(inp.inpid)
                FROM par3.iniciativa_planejamento as inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE inpstatus = 'A'
                     AND inp.inuid = {$array['inuid']} 
                     AND esd.esdid = 1990 ";

        $result = pg_fetch_array($db->executar($sql));
        return (int)$result[0];
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $inuid
     * @return bool
     * @description Esta função retorna true, caso exista iniciativativa cadastrada para o munícipio, false caso não exista nenhuma!
     */
    public function existeIniciativaJaCadastrada($inuid)
    {
        $sql = "select * from par3.iniciativa_planejamento as iniplan
                where iniplan.inuid = {$inuid};";

        try {
            $arrResult = $this->carregar($sql);
            if (isset($arrResult[0]['inpid'])) {
                return true;
            }
        } catch (Exception $e) {
            echo $e->getMessage();
        }

        return false;
    }

    /*
    *   Função pega situação cadastradas em iniciativa para monta select
    */
    public function getSituacaoIniciativa($filtro = '')
    {
        $sql = "SELECT DISTINCT esd.esdid as codigo, esd.esddsc as descricao
                --(CASE WHEN esd.esdid = 1990 THEN esd.esddsc ELSE 'Finalizada' END) as descricao
                FROM par3.iniciativa_planejamento as inp
                INNER JOIN par3.iniciativa as ini USING (iniid)
                INNER JOIN par3.iniciativa_descricao as ind USING (indid)
                INNER JOIN workflow.documento doc USING (docid)
                INNER JOIN workflow.estadodocumento esd USING (esdid)
                WHERE inp.inpstatus = 'A' {$filtro} ORDER BY esd.esddsc ASC";

        return $sql;
    }
    public function getSituacaoAnalise()
    {
        $sql = "SELECT
					e.esdid as codigo,
				    e.esdordem || ' - ' || e.esddsc as descricao --, e.esdordem
				FROM 
        			workflow.estadodocumento e 
				WHERE 
        			e.tpdid = 310 and 
        			e.esdstatus = 'A'
				ORDER BY 
        			e.esdordem asc";
        
        return $sql;
    }
    public function getTecnicoAnalise()
    {
        $sql = "SELECT distinct
					u.usucpf as codigo,
				    u.usunome as descricao
				FROM par3.analise a
					inner join seguranca.usuario u on u.usucpf = a.cpftecnico
				WHERE a.anastatus = 'A'
				order by u.usunome";
        return $sql;
    }
    public function getInicitivaAno()
    {
        $sql = "SELECT distinct
					a.iniano as codigo,
				    a.iniano as descricao
				FROM
					par3.iniciativa_iniciativas_anos a
				WHERE a.iianostatus = 'A'
				order by a.iniano";
        return $sql;
    }
    public function getTipos_Assistencia()
    {
        $sql = "SELECT intaid AS codigo, intadsc AS descricao FROM par3.iniciativa_tipos_assistencia WHERE intastatus = 'A'";
        return $sql;
    }

    public function getValorTotalPlanejamento($arrPost)
    {
        global $db;
        $sql = " SELECT DISTINCT
                     to_char(COALESCE(sum(inpvalor_planejado_total),0), 'L9G999G999G990D99')  as valortotalplanejamento
                FROM par3.iniciativa_planejamento as inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE inpstatus = 'A'
                     AND inp.inuid = {$arrPost['inuid']}
                    --AND esd.esdid = 1991
                     AND esd.esdid IN (".PAR3_ESDID_AGUARDANDO_ANALISE.", ".PAR3_ESDID_EM_ANALISE.", ".PAR3_ESDID_EM_DILIGENCIA.") 
                    ";

        $result = pg_fetch_array($db->executar($sql));
        return $result[0];
    }

    /*
    *   Função pega Estados e Municipios cadastradas em iniciativa para monta select
    */
    public function getEstadoMunicipioPorIniciativa($municipio = false)
    {
        if ($municipio) {
            $sql = "SELECT DISTINCT ins.muncod as codigo, estuf ||' - '|| ins.inudescricao as descricao FROM par3.iniciativa_planejamento ip";
        } else {
            $sql = "SELECT DISTINCT es.estuf as codigo, es.estdescricao as descricao FROM par3.iniciativa_planejamento ip";
        }

        $sql .= " INNER JOIN  par3.instrumentounidade ins USING (inuid)
                INNER JOIN territorios.estado es USING (estuf)
                WHERE ip.inpstatus = 'A'
                ORDER BY 2";
        return $sql;
    }

    public function acessoRapidoCarregarComboMunicipio($arParam=array(), $boolSql=true)
    {
        $where=array();
        if ($arParam['estuf']) {
            $arParam['estuf'] = (array) $arParam['estuf'];
            $where[] = "es.estuf='".implode("', '", $arParam['estuf'])."'";
        }
                
        $sql .= "SELECT DISTINCT 
                    ins.muncod as codigo, 
                    ins.inudescricao ||' - '|| es.estuf as descricao 
                 FROM 
                    par3.iniciativa_planejamento ip
                INNER JOIN  par3.instrumentounidade ins USING (inuid)
                INNER JOIN territorios.estado es USING (estuf)
                WHERE 
                    ip.inpstatus = 'A' AND
                    ins.itrid = 2
                ".($where ? " AND " . implode(" and ", $where) : "")."
                ORDER BY 2";
        
        if ($boolSql == true) {
            return $sql;
        }
        $dados = $this->carregar($sql);
        
        return ($dados ? $dados : array());
    }
    
    /*
    *   Função pega Tipo de Objeto cadastradas em iniciativa para monta select
    */
    public function getTipoObjetoporIniciativa($and = array())
    {
        $andWhere = $and?(count($and) > 1 ? implode(' AND ', $and):' AND '.$and[0]) : '';
        $sql = "SELECT DISTINCT it.intoid as codigo, it.intoid ||' - '||it.intodsc as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa ini USING (iniid)
                INNER JOIN par3.iniciativa_tipos_objeto it USING (intoid)
                WHERE ip.inpstatus = 'A'
                {$andWhere}
                ORDER BY 1";
        return $sql;
    }

    /*
    *   Função pega Tipo Atendimento cadastradas em iniciativa para monta select
    */
    public function getTipoAtendimentoIniciativa()
    {
        $sql = "SELECT DISTINCT it.intaid as codigo, it.intadsc as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa ini USING (iniid)
                INNER JOIN par3.iniciativa_tipos_atendimento it USING (intaid)
                WHERE ip.inpstatus = 'A'
                ORDER BY 2";
        return $sql;
    }

    /*
    *   Função pega Programa cadastradas em iniciativa para monta select
    */
    public function getProgramaIniciativa()
    {
        $sql = "SELECT DISTINCT iip.prgid as codigo, pr.prgdsc as descricao 
        		FROM par3.iniciativa_planejamento ip
                	INNER JOIN par3.iniciativa ini USING (iniid)
                	INNER JOIN par3.iniciativa_iniciativas_programas iip USING (iniid)
                	INNER JOIN par3.programa pr USING (prgid)
                WHERE ip.inpstatus = 'A'
                ORDER BY 2";
        return $sql;
    }

    /*
   *   Função pega Etapa cadastradas em iniciativa para monta select
   */
    public function getEtapaIniciativa()
    {
        $sql = "SELECT DISTINCT ee.etaid as codigo, ee.etadsc as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa ini USING (iniid)
                INNER JOIN par3.ensino_etapa ee USING (etaid)
                WHERE ip.inpstatus = 'A'
                ORDER BY 2";
        return $sql;
    }

    /*
   *   Função pega Desdobramento cadastradas em iniciativa para monta select
   */
    public function getDesdobramentoIniciativa()
    {
        $sql = "SELECT DISTINCT id.desid as codigo, id.desdsc as descricao  FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa_planejamento_desdobramentos ipd USING (inpid)
                INNER JOIN par3.iniciativa_desdobramento id USING (desid)
                WHERE ip.inpstatus = 'A'
                ORDER BY 2";
        return $sql;
    }

    /*
  *   Função pega escolas cadastradas em iniciativa para monta select
  */
    public function getEscolaIniciativa($muncod = null)
    {
        if ($muncod) {
            $muncod = "AND muncod = '{$muncod}'";
        }
        $sql = "SELECT DISTINCT es.escid as codigo, es.esccodinep || ' - ' ||es.escnome as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa ini USING (iniid)
                INNER JOIN par3.iniciativa_planejamento_item_composicao USING (inpid)
                INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ipic USING (ipiid)
                INNER JOIN par3.escola es USING (escid)
                WHERE ip.inpstatus = 'A'
                AND esc.esc.escco_esfera in('M','E')
                AND esc.escco_situacao_funcoinamento in(1,2)
                {$muncod}
                ORDER BY 2";

        return $sql;
    }


    public function verificarExisteIniciativa($inuid)
    {
        $sql = "SELECT count(*)
                FROM {$this->stNomeTabela} inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE inuid = {$inuid} AND inpstatus = 'A'
                AND   esd.esdid = 1990";

        $qtd = $this->recuperar($sql);
        if ($qtd['count'] > 0) {
            return true;
        }
        return false;
    }

    public function verificarSituacaoIniciativa($arrPost)
    {
        //Valida se há emenda vinculada a iniciativa
        $possuiEmenda = $this->carregar("select inpid from par3.iniciativa_emenda WHERE inpid = {$arrPost["inpid"]} AND inestatus = 'A' ");
        if ($possuiEmenda) {
            return "erroEmenda";
        }

        $sql = "SELECT esd.esdid FROM par3.iniciativa_planejamento inp
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid 
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                where inp.inuid = ".$arrPost["inuid"]." and inpid = ".$arrPost["inpid"]."";
        $result = $this->recuperar($sql);
        
        //se não existir a condição, faça a busca em obras
        if ($result["esdid"] == PAR3_ESDID_CADASTRAMENTO || $result["esdid"] == PAR3_AEDID_CANCELADA) {
            $sqlObras = "SELECT DISTINCT esd.esdid FROM par3.obra obr
                         INNER JOIN workflow.documento doc ON doc.docid = obr.docid 
                         INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
                         WHERE obr.inuid = ".$arrPost["inuid"]." and obr.inpid = ".$arrPost["inpid"]."";
            $resultObras = $this->carregar($sqlObras);
           
            if (!$resultObras[0]) {
                 return "sucesso";
            }

            $arr =  array();
            for ($i=1; $i<=count($resultObras); $i++) {
                    array_push($arr, $resultObras[$i-1]['esdid']);
            }


            $erroEncontrado = false;
            foreach ($arr as $ar) {
                if ($ar != "2007") {
                    $erroEncontrado = true;
                }
            }

            if ($erroEncontrado) {
                return "erroObras";
                exit;
            } else {
                    $sqlUpdate = "UPDATE par3.obra SET obrstatus = 'I' WHERE inuid = ".$arrPost["inuid"]." and inpid = ".$arrPost["inpid"]."";
                    $this->carregar($sqlUpdate);
                    return "sucesso";
                    exit;
            }
        } else {
            return "erroIniciativa";
        }
    }

    public function getEstadosIniciativas()
    {
        return  "SELECT
					estadodocumento.esdid as codigo,
					estadodocumento.esddsc  as descricao
					FROM
					workflow.estadodocumento
					WHERE
					workflow.estadodocumento.tpdid =  '302'
                    AND workflow.estadodocumento.esdstatus = 'A'
					ORDER BY esdordem ASC;";
    }

    public function sqlListaDocumentoPlanejamento($inpid)
    {
        $sql = "SELECT
                  arq.arqid,
                  arqnome || '.' || arqextensao as arqnome,
                  ipadsc,
                  to_char(ipadtinclusao, 'dd/mm/YYYY') as datainclusao,
                  doc.esdid
                FROM par3.iniciativa_planejamento inp 
                LEFT JOIN par3.iniciativa_planejamento_arquivos ipa ON inp.inpid = ipa.inpid AND ipastatus = 'A'
                LEFT JOIN arquivo arq ON arq.arqid = ipa.arqid
                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
                WHERE inp.inpid = {$inpid}
                ";
        return $this->carregar($sql);
    }

    public function salvarDocumentoPlanejamento($inpid, $arqid, $dsc)
    {
        if ($dsc) {
            $dsc = ($dsc);
            $inputDsc = 'ipadsc,';
            $valueDsc = "'{$dsc}',";
        }
        $sql = "INSERT INTO par3.iniciativa_planejamento_arquivos (inpid, arqid, {$inputDsc} usucpf) 
               VALUES ({$inpid}, {$arqid}, {$valueDsc} '{$_SESSION['usucpf']}')
               RETURNING ipaid";
        $this->executar($sql);
        return $this->commit();
    }

    public function removerDocumentoPlanejamento($arqid)
    {
        $sql = "UPDATE par3.iniciativa_planejamento_arquivos SET ipastatus = 'I' WHERE arqid = {$arqid}";
        $this->executar($sql);
        return $this->commit();
    }
    
    public function getDimensaoIniciativas($inuid){
        $sql = "SELECT
                  distinct dim.dimid, dim.dimcod, dim.dimdsc
                FROM {$this->stNomeTabela} as inp
                INNER JOIN par3.dimensao dim ON dim.dimid = inp.dimid AND dim.dimstatus = 'A'
                WHERE inp.inuid = {$inuid} AND
                      inpstatus = 'A'
                ORDER BY
                    dim.dimcod
                ";
        return $this->carregar($sql);
    }
    
    public function getDimensaoIniciativasPlanejamento($inuid, $dimid){
        $sql = "SELECT
                  inp.*, ind.inddsc
                FROM {$this->stNomeTabela} as inp
                INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid
                INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid AND ind.indstatus = 'A'
                WHERE inp.inuid = {$inuid} AND
                      inp.dimid = {$dimid} AND
                      inpstatus = 'A'
                ORDER BY
                    inp.iniid
                ";
        return $this->carregar($sql);
    }

}
