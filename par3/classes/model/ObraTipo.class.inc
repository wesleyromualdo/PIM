<?php

/**
 * Classe de mapeamento da entidade par3.obra_tipo.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_tipo: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $otpid  - default: nextval('par3.obra_tipo_otpid_seq'::regclass)
 * @property int $octid
 * @property string $otpdsc
 * @property string $otpsituacao
 * @property string $otpstatus
 * @property string $otpesfera
 * @property string $otpdetalhe
 * @property smallint $otpalunosatendidos
 */
class Par3_Model_ObraTipo extends Modelo
{
    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';
    const CONSTRUCAO = 1;
    const AMPLIACAO = 2;
    const REFORMA = 3;

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'otpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'octid' => array('tabela' => 'par3_obra_categoria', 'pk' => 'octid'),
        'tpoid' => array('tabela' => 'obra2_tipologiaobra', 'pk' => 'tpoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'otpid' => null,
        'octid' => null,
        'tpoid' => null,
        'otpdsc' => null,
        'otpsituacao' => null,
        'otpstatus' => null,
        'otpesfera' => null,
        'otpalunosatendidos' => null,
        'otptipoprojeto' => null,
        'otptipoampliacao' => null,
        'otpdetalhe' => null, //#add
        'igrid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'otpid' => array('Digits'),
            'octid' => array('allowEmpty' => true,'Digits'),
            'otpdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'otpsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'otpstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'otpesfera' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'otpalunosatendidos' => array('allowEmpty' => true),
            'otpdetalhe' => array('allowEmpty' => true),
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

    public function pegarSQLSelectCombo($otpid = null)
    {
        $where = $otpid?"AND otpid = $otpid":'';
        return "SELECT  otpid as codigo,otpdsc as descricao FROM {$this->stNomeTabela} WHERE otpstatus = 'A' $where ORDER BY 2";
    }

    public function pegarSQLSelectComboCategorias($octid = null)
    {
        $where = $octid?"AND octid = $octid":'';
        return "SELECT  octid as codigo,octdsc as descricao FROM par3.obra_categoria WHERE octstatus = 'A' $where ORDER BY 2";
    }

    public function getComboTipologiaObras2(){
        $sql = <<<SQL
      SELECT tpoid AS codigo, tpodsc AS descricao FROM obras2.tipologiaobra  WHERE tpostatus = 'A'
SQL;
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        $arr = array();

        foreach ($dados as $valor) {
            $arr[$valor['codigo']] = $valor['descricao'];
        }

        return $arr;
    }

    public function getComboGrupoItens(){
        $sql = <<<SQL
      SELECT igr.igrid AS codigo, igr.igrid || ' - ' || igrnome || ' (' || array_to_string(array_agg(intodsc),',') || ')'  AS descricao 
      FROM par3.itenscomposicao_grupos igr
      INNER JOIN par3.itenscomposicao_grupos_tipos_objeto gto ON igr.igrid = gto.igrid
      INNER JOIN par3.iniciativa_tipos_objeto ito ON gto.intoid = ito.intoid
      WHERE igrstatus = 'A'
      GROUP BY igr.igrid
SQL;

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        $arr = array();

        foreach ($dados as $valor) {
            $arr[$valor['codigo']] = $valor['descricao'];
        }

        return $arr;
    }

    public function getTipoObraPoretapa($linhas = false){
       $inpid = validarInteiro($_REQUEST['inpid'])? $_REQUEST['inpid']:'';

         $arrFiltro = array("otpstatus = 'A'", "otpsituacao = 'A'");

        if($_REQUEST['octid']){
            $arrFiltro[] = "octid = {$_REQUEST['octid']}";
        }
        if($_REQUEST['otpsituacao']){
            $arrFiltro[] = "otpsituacao = '{$_REQUEST['otpsituacao']}'";
        }
        if(count($_REQUEST['NOTIN'])>0){
            foreach($_REQUEST['NOTIN'] as $campo => $valor){
                $arrFiltro[] = "$campo NOT IN ($valor)";
            }
        }
        if($_REQUEST['etaid']){
            $arrFiltro[] = "etaid = {$_REQUEST['etaid']}";
        }
        if($_REQUEST['inpid']){
            $arrJoin[] = "INNER JOIN par3.obra_tipo_ensino_etapa otp ON otp.otpid = t1.otpid";
            $arrFiltro[] = "etaid in (select etaid from par3.iniciativa_planejamento where inpid in ({$inpid}))";
        }

        $sql = "SELECT t1.otpid AS codigo, t1.otpdsc AS descricao FROM par3.obra_tipo t1
                INNER JOIN par3.obra_tipo_ensino_etapa otp ON otp.otpid = t1.otpid
                WHERE ".implode(' AND ', $arrFiltro)." ";

        if($linhas){
            return $this->pegaLinha($sql);
        }else{
            return $this->carregar($sql);
        }

    }

    public function montarSQLSimples($arrPost)
    {
        $otpdsc     = sanitizar_string_pesquisa($arrPost['otpdsc']);
        $octdsc     = sanitizar_string_pesquisa($arrPost['octdsc']);
        $tpodsc     = sanitizar_string_pesquisa($arrPost['tpodsc']);
        $otpdetalhe = sanitizar_string_pesquisa($arrPost['otpdetalhe']); //#add

        $where = array("otpstatus = 'A'");

        if($arrPost['octdsc'])      $where[] = "octdsc ILIKE '%{$octdsc}%'";
        if($arrPost['tpodsc'])      $where[] = "tpodsc ILIKE '%{$tpodsc}%'";
        if($arrPost['otpdetalhe'])  $where[] = "tpodsc ILIKE '%{$otpdetalhe}%'"; //#add

        if($arrPost['otpsituacao'])                                 $where[] = "otpsituacao = '{$arrPost['otpsituacao']}'";
        if($arrPost['otpesfera'])                                   $where[] = "otpesfera = '{$arrPost['otpesfera']}'";
        if($arrPost['otpalunosatendidos'])                          $where[] = "otpalunosatendidos = '{$arrPost['otpalunosatendidos']}'";
        if($arrPost['otpid'] && validarInteiro($arrPost['otpid']) ) $where[] = "otpid = {$arrPost['otpid']}";
        if($arrPost['octid'] && validarInteiro($arrPost['octid']) ) $where[] = "ot.octid = {$arrPost['octid']}";
        if($arrPost['tpoid'] && validarInteiro($arrPost['tpoid']) ) $where[] = "tpo.tpoid = {$arrPost['tpoid']}";

        if($arrPost['otpdsc']) $where[] = "removeacento(upper(otpdsc)) ILIKE removeacento(upper('%{$arrPost['otpdsc']}%'))";

        $orderBy = $arrPost['ordenacao'] ? "{$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']} ": "otpdsc ";

        $sql = "SELECT
                    otpid AS id,
                    ROW_NUMBER() OVER (ORDER BY $orderBy) AS qtd,

                    otpid AS codigo,
                    otpdsc as descricao,

                    tpodsc,

                    CASE otpsituacao
                      WHEN 'A' THEN 'Ativo'
                      WHEN 'I' THEN 'Inativo'
                      ELSE NULL
                    END AS situacao,

                    CASE otpesfera
                      WHEN 'E' THEN 'Estadual'
                      WHEN 'M' THEN 'Municipal'
                      WHEN 'T' THEN 'Todos'
                    END AS esfera,

                    octdsc AS categoria,
                    otpalunosatendidos AS alunosAtendidos,
                    CASE
                        WHEN position('<img' in otpdetalhe) > 0 THEN substring(otpdetalhe from 0 for position('<img' in otpdetalhe) )
                        ELSE otpdetalhe
                    END as otpdetalhe,
                	CASE otptipoprojeto
                		WHEN 'PF' THEN 'Projeto FNDE'
                		WHEN 'PP' THEN 'Projeto Próprio'
                	END AS otptipoprojeto
                FROM {$this->stNomeTabela} ot
                INNER JOIN par3.obra_categoria oc ON oc.octid = ot.octid
                INNER JOIN obras2.tipologiaobra tpo ON tpo.tpoid = ot.tpoid
                WHERE
                    ".implode(' AND ', $where)."
                ORDER BY
                    $orderBy";

        return $sql;
    }

    public function getOrdenacaoLista($arrPost){
        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;

        switch ($campo) {
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'otpid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'otpdsc';
                break;
            case 'situacao':
                $arrQuery['ordenacao']['campo'] = 'otpsituacao';
                break;
            case 'esfera':
                $arrQuery['ordenacao']['campo'] = 'otpesfera';
                break;
            case 'categoria':
                $arrQuery['ordenacao']['campo'] = 'octdsc';
                break;
            case 'alunosAtendidos':
                $arrQuery['ordenacao']['campo'] = 'otpalunosatendidos';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }
        return $arrQuery;
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup>
     * @param $otpid
     * @return string
     * @description 'Est função retorna o sql para montar o simec->selec()'
     */
        public function getSQLInputTipoObrasTipo($otpid){
            if(is_null($otpid)){
                $sql = "select 
                        obratipo.otpid as codigo,
                        obratipo.otpdsc as descricao
                          from par3.obra_tipo as obratipo
                          where otpsituacao = 'A' and otpstatus = 'A'
                          order by obratipo.otpdsc asc;";
            }else{
                $sql =  "select 
                        obratipo.otpid as codigo,
                        obratipo.otpdsc as descricao
                          from par3.obra_tipo as obratipo
                          where otpsituacao = 'A' and otpstatus = 'A' and obratipo.otpid = {$otpid}
                          order by obratipo.otpdsc asc;";
            }
            return $sql;
        }

        public function getCategoriaPorTipo($otpid)
        {
            if(!$otpid){
                return null;
            }
            $sql =<<<SQL
          select oct.* from par3.obra_categoria oct
          inner join par3.obra_tipo otp on otp.octid = oct.octid
          where otpid = {$otpid}
          and octstatus = 'A'
SQL;
            return $this->pegaLinha($sql);
        }
}


?>