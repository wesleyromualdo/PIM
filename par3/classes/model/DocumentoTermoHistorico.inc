<?php
/**
 * Classe de mapeamento da entidade par3.documentotermo_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_DocumentotermoHistorico: sem descricao
 *
 * @package Model\DocumentotermoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_DocumentoTermoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_DocumentotermoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hdoid chave primária da tabeladocumentotermo_historico
 * @property  $hdocpf CPF do respnsável pela alteração
 * @property  $hdoacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hdodtcriacao Data de criação do log
 * @property int $dotid  - default: nextval('par3.documentotermo_dotid_seq'::regclass)
 * @property int $proid 
 * @property int $mdoid 
 * @property int $dotidpai 
 * @property int $dotnumero 
 * @property string $dotstatus A - Ativo
I - Inativo
B - Bloqueado - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $dotdatainclusao 
 * @property string $usucpfinclusao 
 * @property \Datetime(Y-m-d H:i:s) $dotdataalteracao 
 * @property string $usucpfalteracao 
 * @property numeric $dotvalortermo 
 * @property int $intoid Tipo do Objeto da Iniciativa
 * @property \Datetime(Y-m-d H:i:s) $dotdatainiciovigencia 
 * @property \Datetime(Y-m-d H:i:s) $dotdatafimvigencia 
 * @property \Datetime(Y-m-d H:i:s) $dotdataexclusao Data da Exclusão do Termo
 * @property string $dotusucpfexcluxao Cpf de quem fez a exclusão
 * @property string $dotrejeitado  - default: 'N'::bpchar
 * @property numeric $dotvalorcontrapartida 
 */
class Par3_Model_DocumentoTermoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.documentotermo_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hdoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'dotid' => array('tabela' => 'par3.documentotermo', 'pk' => 'dotid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hdoid' => null,
        'hdocpf' => null,
        'hdoacao' => null,
        'hdodtcriacao' => null,
        'dotid' => null,
        'proid' => null,
        'mdoid' => null,
        'dotidpai' => null,
        'dotnumero' => null,
        'dotstatus' => null,
        'dotdatainclusao' => null,
        'usucpfinclusao' => null,
        'dotdataalteracao' => null,
        'usucpfalteracao' => null,
        'dotvalortermo' => null,
        'intoid' => null,
        'dotdatainiciovigencia' => null,
        'dotdatafimvigencia' => null,
        'dotvalorcontrapartida' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Documentotermo $model,$acao)
    {

        $this->arAtributos['hdocpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hdoacao'] = $acao;
        $this->arAtributos['hdodtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['dotid'] = $this->arAtributos['dotid'];
        $this->arAtributos['proid'] = $model->proid;
        $this->arAtributos['mdoid'] = $model->mdoid;
        $this->arAtributos['dotidpai'] = $model->dotidpai;
        $this->arAtributos['dotnumero'] = $model->dotnumero;
        $this->arAtributos['dotstatus'] = $model->dotstatus;
        $this->arAtributos['dotdatainclusao'] = $model->dotdatainclusao;
        $this->arAtributos['usucpfinclusao'] = $model->usucpfinclusao;
        $this->arAtributos['dotdataalteracao'] = $model->dotdataalteracao;
        $this->arAtributos['usucpfalteracao'] = $model->usucpfalteracao;
        $this->arAtributos['dotvalortermo'] = $model->dotvalortermo;
        $this->arAtributos['intoid'] = $model->intoid;
        $this->arAtributos['dotdatainiciovigencia'] = $model->dotdatainiciovigencia;
        $this->arAtributos['dotdatafimvigencia'] = $model->dotdatafimvigencia;
        $this->arAtributos['dotvalorcontrapartida'] = $model->dotvalorcontrapartida;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
