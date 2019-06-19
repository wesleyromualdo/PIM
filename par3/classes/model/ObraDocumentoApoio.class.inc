<?php
/**
 * Classe de mapeamento da entidade par3.obra_documento_apoio.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_documento_apoio: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_documento_apoio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_documento_apoio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $odaid  - default: nextval('par3.obra_documento_apoio_odaid_seq'::regclass)
 * @property int $arqid
 * @property string $odadsc
 * @property string $odasituacao
 * @property string $odastatus
 */
class Par3_Model_ObraDocumentoApoio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_documento_apoio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'odaid',
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
        'odaid' => null,
        'arqid' => null,
        'odadsc' => null,
        'odasituacao' => null,
        'odastatus' => null,
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
            'odaid' => array('Digits'),
            'arqid' => array('allowEmpty' => true,'Digits'),
            'odadsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'odasituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'odastatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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


    //NOVO

    public function montarSQLSimples($arrPost)
    {
        $odadsc = sanitizar_string_pesquisa($arrPost['odadsc']);
        $where  = $arrPost['odadsc']?" AND upper(removeacento(oda.odadsc)) LIKE '%{$odadsc}%'":'';
        $where .= $arrPost['odasituacao']?" AND oda.odasituacao = '{$arrPost['odasituacao']}'":'';
        $where .= $arrPost['odaid'] && validarInteiro($arrPost['odaid'])?" AND oda.odaid = {$arrPost['odaid']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";
        $sql = "SELECT 
                oda.odaid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                oda.odaid as codigo, oda.odadsc as descricao, arq.arqnome||'.'||arq.arqextensao arquivo, odah.hodcpf, usu.usunome,odah.hoddtcriacao, oda.odasituacao, odahinativacao.hoddtcriacao as hoddtcriacaoinativacao, arq.arqid
                FROM par3.obra_documento_apoio oda
                JOIN par3.obra_documento_apoio_historico odah ON ((SELECT MIN(hodid) FROM par3.obra_documento_apoio_historico hist WHERE hist.odaid = oda.odaid) = odah.hodid )
                LEFT JOIN par3.obra_documento_apoio_historico odahinativacao ON ((SELECT MAX(hodid) FROM par3.obra_documento_apoio_historico hist WHERE hist.odaid = oda.odaid AND odasituacao = 'I') = odahinativacao.hodid )
                JOIN seguranca.usuario usu ON (odah.hodcpf = usu.usucpf)
		        JOIN public.arquivo arq ON (arq.arqid = oda.arqid)
                WHERE oda.odastatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function getIniciativa(array $arrPost) {
        $where = $arrPost['odaid']?" AND odaid != {$arrPost['odaid']}":'';
        return $this->pegaUm("SELECT * FROM {$this->stNomeTabela} WHERE upper(odadsc) = '".str_to_upper($arrPost['odadsc'])."' AND odastatus = 'A' {$where}");
    }

    public function pegarSQLSelectCombo($indid = array())
    {
        $indid = is_array($indid) ? implode(',',$indid):$indid;
        $where = "WHERE indstatus = 'A'";
        $where  = $indid ? " AND indid in({$indid})":'';

        return "SELECT  indid as codigo, inddsc as descricao FROM {$this->stNomeTabela} $where";
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['odadsc'] = array();

        //VALIDA CAMPOS
        if(strlen($campos['odadsc']) > 255){array_push($erros['erros']['odadsc'],'Limite de caracteres excedido');}
        if($campos['odadsc'] == '' || $campos['odadsc'] == null){array_push($erros['erros']['odadsc'],'O campo não pode ser vazio');}
        //if($this->getIniciativa($campos)){array_push($erros['erros']['odadsc'],'A descrição informada já existe');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

}
