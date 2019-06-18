<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_item_composicao_escola.
 *
 * @version $Id$
 * @since 2017.07.17
 */

/**
 * Par3_Model_Iniciativa_planejamento_item_composicao_escola: Tabela utilizada para cadastro dos Itens de composição das Iniciativas por escola
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicao_escola($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicao_escola($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $ipestatus status A- ativo, I - inativo - default: 'A'::bpchar
 * @property int $ipequantidade Quantidade de itens
 * @property int $ipiid FK iniciativa_planejamento_item_composicao
 * @property int $ipeid Chave Primária - default: nextval('par3.iniciativa_planejamento_item_composicao_escola_ipeid_seq'::regclass)
 */
class Par3_Model_IniciativaPlanejamentoItemComposicaoEscola extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_item_composicao_escola';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ipiid' => array('tabela' => 'par3.iniciativa_planejamento_item_composicao', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipestatus' => null,
        'ipequantidade' => null,
        'ipiid' => null,
        'ipeid' => null,
        'co_entidade' => null,
        'escid' => null,
    );
    
    public function pegaIpeidPorEscola($ipiid, $escid)
    {
        $sql = "select
                    ipeid
                from
                    par3.iniciativa_planejamento_item_composicao_escola
                where
                    ipiid = {$ipiid} and
                    escid = {$escid}";
        $ipeid = $this->pegaUm($sql);
        
        return ($ipeid ? $ipeid : false);
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
            'ipestatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'ipequantidade' => array('allowEmpty' => true, 'Digits'),
            'ipiid' => array('Digits'),
            'ipeid' => array('Digits'),
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

    public function getDadosTotais($dados){
        $sql = "SELECT SUM(ipequantidade) as ipiquantidade
                  FROM par3.iniciativa_planejamento_item_composicao
                  JOIN par3.iniciativa_planejamento_item_composicao_escola USING (ipiid)
                  WHERE ipestatus = 'A' and escid is not null AND ".implode(" AND ", $dados);
        return $this->carregar($sql);
    }

    public function getDadosEscolasPAR3($dados)
    {
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $desdobramentos = $inp->getDesdobramentos($_REQUEST['inpid']);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($_REQUEST['inpid']);

        switch($mdPlanejamento->etaid){
        	case '1':
                $colunaQtdAlunos = 'escqtd_alunos_infantil';
        	    break;
        	case '2':
                $colunaQtdAlunos = 'escqtd_alunos_fundamental';
        	    break;
        	case '3':
                $colunaQtdAlunos = 'escqtd_alunos_medio';
        	    break;
            case '5':
                $colunaQtdAlunos = '(escqtd_alunos_medio+escqtd_alunos_fundamental+escqtd_alunos_infantil)';
                break;
        }

        $filtroLocalizacao = '';
        $filtroLocalizacao = 'AND escola.escco_localizacao IN(1,2)';
        //Verifica se existe desdobramento urbano
        if (in_array(DESDOBRAMENTO_URBANO, $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao = 1';
        }

        //Verifica se existe desdobramento rural
        if (in_array(DESDOBRAMENTO_RURAL, $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao = 2';
        }

        //Verifica se existe desdobramento urbano e rural na iniciativa
        if (in_array(array(DESDOBRAMENTO_URBANO, DESDOBRAMENTO_RURAL), $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao IN(1,2)';
        }
        $muncod = $dados['muncod']?" AND mun.muncod = '{$dados['muncod']}' ":'';
        $mInu = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $validarItrid = $mInu->itrid == 2?"AND escola.escco_esfera = 'M'":"AND escola.escco_esfera = 'E'";
        
        $join 			= '';
        $colQtdEmenda 	= '';
        if ( $dados['ineid'] ){
        	$join = "LEFT JOIN (
				        select
	                    	escola.ipequantidade,
	                        escola.escid,
	                        iee.ieequantidade
				        FROM
				        	par3.iniciativa_emenda ie
				        join par3.iniciativa_planejamento ip on ip.inpid = ie.inpid and
				        										ip.inpstatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
				        														ic.ipistatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
				        																   escola.ipestatus = 'A'
				        LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
																				       iee.ineid = ie.ineid and
																				       iee.ieestatus = 'A'
				        WHERE
				        	ie.inestatus = 'A'
				        	AND ie.ineid = {$dados['ineid']}
				        	AND ic.inpid  = {$dados['inpid']}
				        	AND ic.ipiano = {$dados['iniano']}
				       		AND ic.iigid  = {$dados['iigid']}
	                ) AS dadosescola ON (dadosescola.escid = escola.escid)";
        	$colQtdEmenda = "dadosescola.ieequantidade  as qtd_emenda,";
        }else{
        	$join = "LEFT JOIN (
				        select
	                    	escola.ipequantidade,
	                        escola.escid
				        FROM
				        	par3.iniciativa_planejamento ip
				        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
				        														ic.ipistatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
				        																   escola.ipestatus = 'A'
				        WHERE
				        	ip.inpstatus = 'A'
				        	AND ic.inpid  = {$dados['inpid']}
				        	AND ic.ipiano = {$dados['iniano']}
				       		AND ic.iigid  = {$dados['iigid']}
	                ) AS dadosescola ON (dadosescola.escid = escola.escid)";
        }
        $arrEsc = $this->carregarEscidItemComposicaoEscolas($dados);
        $orEscid = '';
        if(!empty($arrEsc)) {
            $arrEsc = array_map(function ($esc){
                return $esc['escid'];
            },$arrEsc);
            $arrEscFilter = array_filter($arrEsc);
            if(count($arrEscFilter) > 0){
                $implodeEsc = implode(',',$arrEscFilter);
                $orEscid = "OR escola.escid IN ({$implodeEsc})";
            }
        }
        $sql = "SELECT DISTINCT ON(escola.escid)
                       mun.mundescricao,
                       escola.escid,
                       escola.escnome,
                       escola.esccodinep,
                       ipequantidade,
                       {$colQtdEmenda}
	                   escola.escno_localizacao,
	                   escola.escco_esfera,
	                   escola.escno_esfera,
                       escqtd_salas_utilizadas as nu_salas_existentes,
                       COALESCE($colunaQtdAlunos,0) as qtd_matricula
                FROM par3.instrumentounidade ins
                INNER JOIN territorios.municipio mun ON ((ins.muncod IS NOT NULL
                     AND ins.muncod = mun.muncod)
                     OR (ins.muncod IS NULL
                     AND ins.estuf = mun.estuf))
                INNER JOIN par3.escola escola ON (mun.muncod::bigint = escola.muncod::bigint AND (escola.escco_situacao_funcoinamento in (1,2) {$orEscid}))
                {$join}
                WHERE
                    ins.inuid={$dados['inuid']}
                    AND COALESCE($colunaQtdAlunos,0) >0
                    AND (
                        (ins.muncod IS NOT NULL)
                        OR (ins.muncod IS NULL)
                    )
                           {$filtroLocalizacao}
                           {$muncod}
                           {$validarItrid}
                ORDER BY escola.escid
                ";
        return $this->carregar($sql);
    }
    
    public function getDadosEscolasPAR3ComVinculoParlamentar($dados)
    {
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $desdobramentos = $inp->getDesdobramentos($_REQUEST['inpid']);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($_REQUEST['inpid']);

        switch($mdPlanejamento->etaid){
        	case '1':
                $colunaQtdAlunos = 'escqtd_alunos_infantil';
        	    break;
        	case '2':
                $colunaQtdAlunos = 'escqtd_alunos_fundamental';
        	    break;
        	case '3':
                $colunaQtdAlunos = 'escqtd_alunos_medio';
        	    break;
            case '5':
                $colunaQtdAlunos = '(escqtd_alunos_medio+escqtd_alunos_fundamental+escqtd_alunos_infantil)';
                break;
        }

        $filtroLocalizacao = '';
        $filtroLocalizacao = 'AND escola.escco_localizacao IN(1,2)';
        //Verifica se existe desdobramento urbano
        if (in_array(DESDOBRAMENTO_URBANO, $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao = 1';
        }

        //Verifica se existe desdobramento rural
        if (in_array(DESDOBRAMENTO_RURAL, $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao = 2';
        }

        //Verifica se existe desdobramento urbano e rural na iniciativa
        if (in_array(array(DESDOBRAMENTO_URBANO, DESDOBRAMENTO_RURAL), $desdobramentos)) {
            $filtroLocalizacao = 'AND escola.escco_localizacao IN(1,2)';
        }
        $muncod = $dados['muncod']?" AND mun.muncod = '{$dados['muncod']}' ":'';
        $mInu = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $validarItrid = $mInu->itrid == 2?"AND escola.escco_esfera = 'M'":"AND escola.escco_esfera = 'E'";

        $arrEsc = $this->carregarEscidItemComposicaoEscolas($dados);
        $orEscid = '';
        if(!empty($arrEsc)) {
            $arrEsc = array_map(function ($esc){
                return $esc['escid'];
            },$arrEsc);
            $arrEscFilter = array_filter($arrEsc);
            if(count($arrEscFilter) > 0){
                $implodeEsc = implode(',',$arrEscFilter);
                $orEscid = "OR escola.escid IN ({$implodeEsc})";
            }
        }
        $sql = "SELECT DISTINCT ON(escola.escid)
                       mun.mundescricao,
                       escola.escid,
                       escola.escnome,
                       escola.esccodinep,
                       dadosescola.ipiid,
                       dadosescola.ipeid,
                       ipequantidade,
                       dadosescola.ieequantidade,
	                   escola.escno_localizacao,
	                   escola.escco_esfera,
	                   escola.escno_esfera,
                       escqtd_salas_utilizadas as nu_salas_existentes,
                       COALESCE($colunaQtdAlunos,0) as qtd_matricula
                FROM
                	par3.instrumentounidade ins
                INNER JOIN territorios.municipio mun ON ((ins.muncod IS NOT NULL
                     AND ins.muncod = mun.muncod)
                     OR (ins.muncod IS NULL
                     AND ins.estuf = mun.estuf))
                INNER JOIN par3.escola escola ON (mun.muncod::bigint = escola.muncod::bigint)
                LEFT JOIN (
                    SELECT
                    	ic.ipiid,
                    	escola.ipeid,
                    	escola.ipequantidade,
                        escola.escid,
                    	iee.ieequantidade
                    FROM
                    	par3.iniciativa_planejamento_item_composicao ic
                    INNER JOIN par3.iniciativa_planejamento_item_composicao_escola escola USING (ipiid)
                    LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
                    															   iee.ieestatus = 'A' and
                    															   iee.ineid = {$dados['ineid']}
                    WHERE
        				ic.inpid  = {$dados['inpid']}
                        AND ic.ipiano = {$dados['iniano']}
                        AND ic.iigid  = {$dados['iigid']}
                        AND escola.ipestatus = 'A'
                        AND ic.ipistatus = 'A'
                ) AS dadosescola ON (dadosescola.escid = escola.escid)
                WHERE
                    ins.inuid={$dados['inuid']}
                    AND COALESCE($colunaQtdAlunos,0) >0
                    AND (
                        (ins.muncod IS NOT NULL)
                        OR (ins.muncod IS NULL)
                    )
                    AND (escola.escco_situacao_funcoinamento IN(1,2) {$orEscid})
                           {$filtroLocalizacao}
                           {$muncod}
                           {$validarItrid}
                ORDER BY escola.escid
                ";

        return $this->carregar($sql);
    }

    /**
     * Retorna array de escolas vinculadas ao planejamento
     * @param $dados
     * @return array
     */
    public function carregarEscidItemComposicaoEscolas($dados)
    {
        $sql = '';
        if ( $dados['ineid'] ) {
            $sql = "
                        select
                            escola.escid
                        FROM
                            par3.iniciativa_emenda ie
                        join par3.iniciativa_planejamento ip on ip.inpid = ie.inpid and
                                                                ip.inpstatus = 'A'
                        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
                                                                                ic.ipistatus = 'A'
                        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
                                                                                           escola.ipestatus = 'A'
                        LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
                                                                                       iee.ineid = ie.ineid and
                                                                                       iee.ieestatus = 'A'
                        WHERE
                            ie.inestatus = 'A'
                            AND ie.ineid = {$dados['ineid']}
                            AND ic.inpid  = {$dados['inpid']}
                            AND ic.ipiano = {$dados['iniano']}
                            AND ic.iigid  = {$dados['iigid']}
                    ";
        }else {
            $sql = "select
                        escola.escid
                    FROM
                        par3.iniciativa_planejamento ip
                    join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
                                                                            ic.ipistatus = 'A'
                    join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
                                                                                       escola.ipestatus = 'A'
                    WHERE
                        ip.inpstatus = 'A'
                        AND ic.inpid  = {$dados['inpid']}
                        AND ic.ipiano = {$dados['iniano']}
                        AND ic.iigid  = {$dados['iigid']}
            ";
        }
        $arrEsc = $this->carregar($sql);
        if(!$arrEsc) {
            return array();
        }
        return (!empty(array_filter($arrEsc)) ? $arrEsc :array());
    }

    public function carregarEscolasEmendas($dados)
    {
        $sql = "
            SELECT
                ic.ipiid,
                escola.ipeid,
                escola.ipequantidade,
                escola.escid,
                iee.ieequantidade
            FROM
                par3.iniciativa_planejamento_item_composicao ic
            INNER JOIN par3.iniciativa_planejamento_item_composicao_escola escola USING (ipiid)
            LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
                                                                           iee.ieestatus = 'A' and
                                                                           iee.ineid = {$dados['ineid']}
            WHERE
                ic.inpid  = {$dados['inpid']}
                AND ic.ipiano = {$dados['iniano']}
                AND ic.iigid  = {$dados['iigid']}
                AND escola.ipestatus = 'A'
                AND ic.ipistatus = 'A'
        ";
        $arrEsc = $this->carregar($sql);
        if(!$arrEsc) {
            return array();
        }
        return (!empty(array_filter($arrEsc)) ? $arrEsc :array());
    }
}

